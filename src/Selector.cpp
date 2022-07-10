#include "Selector.hpp"

Selector::Selector() {}

Selector::~Selector() {}

Selector::SocketMap &Selector::getReadySockets() { return ready_all_; }

Selector::SocketMap &Selector::getReadyReadSockets() { return ready_read_; }

Selector::SocketMap &Selector::getReadyWriteSockets() { return ready_write_; }

void Selector::clear() {
  ready_all_.clear();
  ready_read_.clear();
  ready_write_.clear();
  target_read_.clear();
  target_write_.clear();
}

void Selector::init(const SocketMap &target) {
  for (SocketMap::const_iterator it = target.begin(); it != target.cend();
       it++) {
    if (utils::isServerSocket(it->second)) {
      target_read_[it->first] = it->second;
    } else {
      addTarget(dynamic_cast<Connection *>(it->second));
    }
  }
}

int Selector::select(const SocketMap &fd2socket) {
  clear();
  init(fd2socket);

  int maxfd = getMaxFd();
  fd_set readfds = toFdset(target_read_);
  fd_set writefds = toFdset(target_write_);
  struct timeval timeout = {kTimeoutSec, 0};

  int event_count = ::select(maxfd + 1, &readfds, &writefds, NULL, &timeout);
  if (event_count < 0) {
    throw std::runtime_error("select() failed");
  } else if (event_count == 0) {
    std::cerr << "Time out, you had tea break?" << std::endl;
  } else {
    ready_read_ = toSocketMap(readfds, target_read_);
    ready_write_ = toSocketMap(writefds, target_write_);
    ready_all_ = ready_read_;
    ready_all_.insert(ready_write_.begin(), ready_write_.end());
  }

  // デバッグ用
  showTarget();

  return event_count;
}

void Selector::showTarget() {
  std::cerr << "###Read Target  : ";
  for (SocketMap::const_iterator it = target_read_.begin();
       it != target_read_.cend(); it++) {
    std::cerr << it->first << ", ";
  }
  std::cerr << std::endl;
  std::cerr << "###Write Target : ";
  for (SocketMap::const_iterator it = target_write_.begin();
       it != target_write_.cend(); it++) {
    std::cerr << it->first << ", ";
  }
  std::cerr << std::endl << std::endl;
}

int Selector::getMaxFd() {
  int maxfd = -1;
  int maxfd_read = -1;
  int maxfd_write = -1;

  if (!target_read_.empty()) {
    maxfd_read = target_read_.rbegin()->first;
  }
  if (!target_write_.empty()) {
    maxfd_write = target_write_.rbegin()->first;
  }
  maxfd = (maxfd_read > maxfd_write) ? maxfd_read : maxfd_write;

  return maxfd;
}

fd_set Selector::toFdset(const SocketMap &sockets) {
  fd_set ret;

  FD_ZERO(&ret);
  for (SocketMap::const_iterator it = sockets.cbegin(); it != sockets.cend();
       it++) {
    FD_SET(it->first, &ret);
  }
  return ret;
}

Selector::SocketMap Selector::toSocketMap(const fd_set &fdset,
                                          const SocketMap &target_fds) {
  SocketMap fd2set;

  for (SocketMap::const_iterator it = target_fds.cbegin();
       it != target_fds.cend(); it++) {
    if (FD_ISSET(it->first, &fdset)) {
      fd2set[it->first] = it->second;
    }
  }
  return fd2set;
}

void Selector::addTarget(Connection *connection) {
  switch (connection->getState()) {
  case Connection::READ:
    target_read_[connection->getFd()] = connection;
    break;
  case Connection::WRITE:
    target_write_[connection->getFd()] = connection;
    break;
  case Connection::CLOSE:
    throw std::runtime_error("unexpected state");
    break;
  }
}
