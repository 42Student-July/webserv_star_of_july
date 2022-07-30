#include "Selector.hpp"

Selector::Selector() {}

Selector::~Selector() {}

void Selector::clear() {
  readfds_.clear();
  writefds_.clear();
  target_readfds_.clear();
  target_writefds_.clear();
}

const std::vector<int> Selector::readFds() const { return readfds_; }
const std::vector<int> Selector::writeFds() const { return writefds_; }

void Selector::select(const ServerSocketMap &server_sock_map_,
                      const ClientSocketMap &client_sock_map_) {
  // selectの準備
  clear();
  storeTargetfds(server_sock_map_, client_sock_map_);

  fd_set readfds = prepareReadfds();
  fd_set writefds = prepareWritefds();
  int maxfd = calcMaxFd();
  struct timeval timeout = {kTimeoutSec, 0};

  // デバッグ用
  showInfo(maxfd);

  // selectの実行
  int count = ::select(maxfd + 1, &readfds, &writefds, NULL, &timeout);
  if (count < 0) {
    throw std::runtime_error("select() failed");
  }
  if (count == 0) {
    std::cerr << "Time out, you had tea break?" << std::endl;
  } else {
    readfds_ = toVector(readfds, maxfd);
    writefds_ = toVector(writefds, maxfd);
  }
}

void Selector::storeTargetfds(const ServerSocketMap &server_sock_map_,
                              const ClientSocketMap &client_sock_map_) {
  for (ServerSocketMap::const_iterator it = server_sock_map_.begin();
       it != server_sock_map_.end(); it++) {
    target_readfds_.push_back(it->first);
  }
  for (ClientSocketMap::const_iterator it = client_sock_map_.begin();
       it != client_sock_map_.end(); it++) {
    // メソッドつくる、ここで == を使わない
    if (it->second->getState() == ClientSocket::READ) {
      target_readfds_.push_back(it->first);
    } else {
      target_writefds_.push_back(it->first);
    }
  }
}

fd_set Selector::prepareReadfds() {
  fd_set readfds;
  FD_ZERO(&readfds);
  for (std::vector<int>::const_iterator it = target_readfds_.begin();
       it != target_readfds_.end(); it++) {
    FD_SET(*it, &readfds);
  }
  return readfds;
}

fd_set Selector::prepareWritefds() {
  fd_set writefds;
  FD_ZERO(&writefds);
  for (std::vector<int>::const_iterator it = target_writefds_.begin();
       it != target_writefds_.end(); it++) {
    FD_SET(*it, &writefds);
  }
  return writefds;
}

int Selector::calcMaxFd() {
  int max_readfd;
  if (target_readfds_.empty()) {
    max_readfd = -1;
  } else {
    max_readfd =
        *std::max_element(target_readfds_.begin(), target_readfds_.end());
  }
  int max_writefd;
  if (target_writefds_.empty()) {
    max_writefd = -1;
  } else {
    max_writefd =
        *std::max_element(target_writefds_.begin(), target_writefds_.end());
  }
  return std::max(max_readfd, max_writefd);
}

std::vector<int> Selector::toVector(const fd_set &fdset, int maxfd) {
  std::vector<int> ret;
  for (int i = 0; i <= maxfd; i++) {
    if (FD_ISSET(i, &fdset)) {
      ret.push_back(i);
    }
  }
  return ret;
}

void Selector::showInfo(int maxfd) {
  std::cerr << "*** Selecter Target Info ***" << std::endl;
  std::cerr << "readfd  : ";
  for (std::vector<int>::const_iterator it = target_readfds_.begin();
       it != target_readfds_.end(); it++) {
    std::cerr << *it << ", ";
  }
  std::cerr << std::endl;
  std::cerr << "writefd : ";
  for (std::vector<int>::const_iterator it = target_writefds_.begin();
       it != target_writefds_.end(); it++) {
    std::cerr << *it << ", ";
  }
  std::cerr << std::endl;
  std::cerr << "maxfd   : " << maxfd << std::endl;
  std::cerr << "****************************" << std::endl;
  std::cerr << std::endl;
}
