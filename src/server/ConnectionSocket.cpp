#include "ConnectionSocket.hpp"

ConnectionSocket::ConnectionSocket(int accepted_fd,
                                   const ServerConfig &serverconfig)
    : ASocket(accepted_fd, serverconfig), state_(READ) {}

ConnectionSocket::~ConnectionSocket() {
  if (close(fd_) < 0) {
    std::runtime_error("close() failed");
  }
}

void ConnectionSocket::handleCommunication() {
  if (state_ == READ) {
    handleReadEvent();
  } else {
    handleWriteEvent();
  }
}

ConnectionSocket::State ConnectionSocket::getState() const { return state_; }

void ConnectionSocket::handleReadEvent() {
  ssize_t recv_size = recvFromClient();

  if (recv_size == 0) {
    state_ = CLOSE;
    return;
  }
  generateRequest(recv_size);
  generateResponse();
  state_ = WRITE;
}

void ConnectionSocket::handleWriteEvent() {
  sendResponse();
  state_ = READ;
}

ssize_t ConnectionSocket::recvFromClient() {
  ssize_t recv_size = recv(fd_, recv_buffer_, kRecvBufferSize, 0);

  if (recv_size < 0) {
    throw std::runtime_error("recv() failed");
  }
  if (recv_size == 0) {
    std::cerr << "recv: EOF" << std::endl << std::endl;
    return 0;
  }
  return recv_size;
}

void ConnectionSocket::generateRequest(ssize_t recv_size) {
  recv_buffer_[recv_size] = '\0';
  current_request_ = request_parser_.parse(recv_buffer_, serverconfig_);

  std::cerr << *current_request_;
}

// // GETメソッドのファイル決め打ち
void ConnectionSocket::generateResponse() {
  ConfigConverter conf_converter;
  ConfigDTO *conf_dto = conf_converter.toDTO(serverconfig_);
  HttpRequestConverter req_converter;
  HttpRequestDTO *req_dto = req_converter.toDTO(*current_request_);
  HttpResponseBuilder builder = HttpResponseBuilder(*conf_dto);
  current_response_ = builder.build(*req_dto);
}

void ConnectionSocket::sendResponse() const {
  HttpResponseSerializer serializer = HttpResponseSerializer();
  HttpResponsePlainText *plain_txt = serializer.serialize(*current_response_);
  const char *response = plain_txt->Text().c_str();
  size_t response_len = plain_txt->Size();

  std::cerr << response;

  if (send(fd_, response, response_len, 0) !=
      static_cast<ssize_t>(response_len)) {
    throw std::runtime_error("send() failed");
  }
}
