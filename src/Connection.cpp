#include "Connection.hpp"

Connection::Connection(int accepted_fd) : ASocket(accepted_fd), state_(READ) {}

Connection::~Connection() {}

void Connection::notifyFdEvent(Observer *observer,
                               std::map<int, ASocket *> *fd2socket) {
  std::cout << "From Connection.notify" << std::endl;
  // std::cout << "fd:" << fd_ << std::endl;
  if (state_ == READ) {
    handleReadEvent(observer, fd2socket);
  } else {
    handleWriteEvent(observer);
  }
}

void Connection::handleReadEvent(Observer *observer,
                                 std::map<int, ASocket *> *fd2socket) {
  ssize_t recv_size = recvFromClient();

  (void)fd2socket;
  if (recv_size == 0) {
    observer->delTargetReadFd(fd_);
    // ここでコネクションオブジェクトを破棄したいけどするとバグ理想
    state_ = CLOSE;
    return;
  }
  std::cerr << "recv from fd(" << fd_ << "): " << std::endl;
  generateRequest(recv_size);
  generateResponse(recv_size);
  observer->addTargetWriteFd(fd_);
  state_ = WRITE;
}

void Connection::handleWriteEvent(Observer *observer) {
  sendResponse();
  observer->delTargetWriteFd(fd_);
  state_ = READ;
}

ssize_t Connection::recvFromClient() {
  ssize_t recv_size = recv(fd_, recv_buffer_, kRecvBufferSize, 0);

  if (recv_size < 0) {
    throw std::runtime_error("recv() failed");
  }
  if (recv_size == 0) {
    std::cerr << "recv: EOF" << std::endl;
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
  printf("[\x1b[32mSEND\x1b[39m]\n");
  const char *response = response_.c_str();
  size_t response_len = response_.size();

  std::cerr << response << std::endl;
  if (send(fd_, response, response_len, 0) !=
      static_cast<ssize_t>(response_len)) {
    throw std::runtime_error("send() failed");
  }
}

// std::string readFile(const char *filename) {
//   ifstream ifs(filename);
//   return std::string(istreambuf_iterator<char>(ifs),
//   istreambuf_iterator<char>());
// }
