#include "Observer.hpp"

Observer::Observer() : maxfd_(-1) {
  FD_ZERO(&readfds_);
  FD_ZERO(&writefds_);
  init();
}

Observer::~Observer() {}

void Observer::init() {
  evnet_cnt_ = 0;
  timeout_.tv_sec = kTimeoutSec;
  timeout_.tv_usec = 0;
}

void Observer::notifyFdEvent() {
  init();

  fd_set tmp_readfds = readfds_;
  fd_set tmp_writefds = writefds_;

  showInfo();
  evnet_cnt_ = select(maxfd_ + 1, &tmp_readfds, &tmp_writefds, NULL, &timeout_);
  if (evnet_cnt_ < 0) {
    throw std::runtime_error("select() failed");
  } else if (evnet_cnt_ == 0) {
    std::cerr << "Time out, you had tea break?" << std::endl;
  }
  readyfds_ = buildReadyFds(tmp_readfds, tmp_writefds);
}

// rbegin, 要素が０のときどうなるか怪しい
void Observer::addTargetReadFd(int fd) {
  FD_SET(fd, &readfds_);
  targetfds_.insert(fd);
  maxfd_ = *targetfds_.rbegin();
}

void Observer::addTargetWriteFd(int fd) {
  FD_SET(fd, &writefds_);
  targetfds_.insert(fd);
  maxfd_ = *targetfds_.rbegin();
}

void Observer::delTargetReadFd(int fd) {
  FD_CLR(fd, &readfds_);
  targetfds_.erase(fd);
  maxfd_ = *targetfds_.rbegin();
}

void Observer::delTargetWriteFd(int fd) {
  FD_CLR(fd, &writefds_);
  targetfds_.erase(fd);
  maxfd_ = *targetfds_.rbegin();
}

const std::set<int>& Observer::getReadyFds() const { return readyfds_; }

const std::set<int>& Observer::getTargetFds() const { return targetfds_; }

std::set<int> Observer::buildReadyFds(fd_set readfds, fd_set writefds) const {
  std::set<int> new_readyfds;
  std::set<int>::iterator it = targetfds_.begin();
  std::set<int>::iterator ite = targetfds_.end();

  for (; it != ite; it++) {
    if (FD_ISSET(*it, &readfds) || FD_ISSET(*it, &writefds)) {
      new_readyfds.insert(*it);
    }
  }
  return new_readyfds;
}

void Observer::printfdset(fd_set fds) const {
  std::set<int>::iterator it = targetfds_.begin();
  std::set<int>::iterator ite = targetfds_.end();

  for (; it != ite; it++) {
    if (FD_ISSET(*it, &fds)) {
      std::cerr << *it << ", ";
    }
  }
  std::cerr << std::endl;
}

void printSetInfo(std::set<int> fds) {
  std::set<int>::iterator it = fds.begin();
  std::set<int>::iterator ite = fds.end();

  std::cerr << "size[" << fds.size() << "] ";
  for (; it != ite; it++) {
    std::cerr << *it << ", ";
  }
  std::cerr << std::endl;
}

void Observer::showInfo() const {
  std::cerr << std::endl;
  std::cerr << "###Observer Info###" << std::endl
            << "max_fd : " << maxfd_ << std::endl
            << "evnet_cnt: " << evnet_cnt_ << std::endl;
  std::cerr << "target_readfds : ";
  printfdset(readfds_);
  std::cerr << "target_writefds: ";
  printfdset(writefds_);
  std::cerr << "readyfds : ";
  printSetInfo(readyfds_);
  std::cerr << "###end###" << std::endl;
  std::cerr << std::endl;
}

// Observer::Observer(const Observer& other) {
//   *this = other;
// }

// Observer& Observer::operator=(const Observer& other) {
//   if (this != &other) {
//   }
//   return *this;
// }
