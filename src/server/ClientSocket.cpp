#include "ClientSocket.hpp"

ClientSocket::ClientSocket(int accepted_fd, const ServerConfig &serverconfig)
    : ASocket(accepted_fd, serverconfig),
      state_(READ),
      current_request_(NULL),
      current_response_(NULL) {}

ClientSocket::~ClientSocket() {
  if (close(fd_) < 0) {
    std::runtime_error("close() failed");
  }
}

bool ClientSocket::isWaitingForRequest() const { return state_ == READ; }

bool ClientSocket::canResponse() const { return state_ == WRITE; }

bool ClientSocket::shouldClose() const { return state_ == CLOSE; }

void ClientSocket::handleReadEvent() {
  ssize_t recv_size = recvFromClient();

  if (recv_size == 0) {
    state_ = CLOSE;
    return;
  }
  generateRequest(recv_size);
  if (request_parser_.finished() || request_parser_.errorOccured()) {
    generateResponse();
    state_ = WRITE;
  }
}

void ClientSocket::handleWriteEvent() {
  sendResponse();
  state_ = READ;
}

ssize_t ClientSocket::recvFromClient() {
  ssize_t recv_size = recv(fd_, recv_buffer_, kRecvBufferSize, 0);

  if (recv_size < 0) {
    throw std::runtime_error("recv() failed");
  }
  if (recv_size == 0) {
    std::cerr << "recv: EOF" << std::endl << std::endl;
    return 0;
  }
  // std::cerr << "recv_buffer_: " << std::endl << recv_buffer_ << std::endl;
  return recv_size;
}

void ClientSocket::generateRequest(ssize_t recv_size) {
  MessageBodyParser body_parser;
  recv_buffer_[recv_size] = '\0';
  request_parser_.parse(recv_buffer_, serverconfig_);
  if (request_parser_.finished() || request_parser_.errorOccured()) {
    current_request_ = request_parser_.buildRequest(serverconfig_);
    std::cerr << *current_request_ << std::endl;
    // std::cerr << current_request_->header.requestLine();
    // std::cerr << current_request_->body;
    // std::cerr << "#status code:" << std::endl
    //           << current_request_->response_status_code << std::endl;
  }
}

// // GETメソッドのファイル決め打ち
void ClientSocket::generateResponse() {
  ConfigConverter conf_converter;
  ConfigDTO *conf_dto = conf_converter.toDTO(serverconfig_);
  HttpRequestConverter req_converter;
  HttpRequestDTO *req_dto = req_converter.toDTO(*current_request_);
  HttpResponseBuilder builder = HttpResponseBuilder(*conf_dto);
  current_response_ = builder.build(*req_dto);
}

void ClientSocket::sendResponse() const {
  HttpResponseSerializer serializer = HttpResponseSerializer();
  HttpResponsePlainText *plain_txt = serializer.serialize(*current_response_);
  const char *response = plain_txt->Text().c_str();
  size_t response_len = plain_txt->Size();

  // std::cerr << response << std::endl;

  if (send(fd_, response, response_len, 0) !=
      static_cast<ssize_t>(response_len)) {
    throw std::runtime_error("send() failed");
  }
}
