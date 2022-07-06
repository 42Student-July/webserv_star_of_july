#include "Connection.hpp"

Connection::Connection(int accepted_fd) : ASocket(accepted_fd), state_(READ) {}

Connection::~Connection() {
  if (close(fd_) < 0) {
    std::runtime_error("close() failed");
  }
}

void Connection::communicateWithClient(SocketObserver *observer) {
  std::cout << "@From Connection  : " << fd_ << ", communicateWithClient"
            << std::endl
            << std::endl;
  if (state_ == READ) {
    handleReadEvent(observer);
  } else {
    handleWriteEvent(observer);
  }
}

void Connection::handleReadEvent(SocketObserver *observer) {
  ssize_t recv_size = recvFromClient();

  if (recv_size == 0) {
    observer->delTarget(this, SocketObserver::READ);
    state_ = CLOSE;
    return;
  }
  generateRequest(recv_size);
  generateResponse(recv_size);
  observer->modTarget(this, SocketObserver::READ, SocketObserver::WRITE);
  state_ = WRITE;
}

void Connection::handleWriteEvent(SocketObserver *observer) {
  sendResponse();
  observer->modTarget(this, SocketObserver::WRITE, SocketObserver::READ);
  state_ = READ;
}

ssize_t Connection::recvFromClient() {
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

void Connection::generateRequest(ssize_t recv_size) {
  recv_buffer_[recv_size] = '\0';
  current_request_ = request_parser_.parse(recv_buffer_);

  std::cerr << *current_request_;
}

// // GETメソッドのファイル決め打ち
void Connection::generateResponse(ssize_t recv_size) {
  recv_buffer_[recv_size] = '\0';

  current_response_ = new HttpResponse();
  std::ifstream ifs("./www/html/index.html");
  std::string str((std::istreambuf_iterator<char>(ifs)),
                  std::istreambuf_iterator<char>());
  current_response_->setBody(str);
  response_ = current_response_->toString();
}

void Connection::sendResponse() const {
  const char *response = response_.c_str();
  size_t response_len = response_.size();

  // std::cerr << response << std::endl;
  if (send(fd_, response, response_len, 0) !=
      static_cast<ssize_t>(response_len)) {
    throw std::runtime_error("send() failed");
  }
}
