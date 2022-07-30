#include "Selector.hpp"

Selector::Selector() {}

Selector::~Selector() {}

const FdVector Selector::readableFds() const { return readable_fds_; }
const FdVector Selector::writableFds() const { return writable_fds_; }

void Selector::select(const ServerSocketMap &serv_socks,
                      const ClientSocketMap &clnt_socks) {
  // selectの準備
  fd_set readfds;
  fd_set writefds;
  setFdset(&readfds, &writefds, serv_socks, clnt_socks);
  int maxfd = calcMaxFd(serv_socks, clnt_socks);
  struct timeval timeout = {kTimeoutSec, 0};

  // デバッグ用
  showInfo(maxfd, serv_socks, clnt_socks);

  // selectの実行
  int count = ::select(maxfd + 1, &readfds, &writefds, NULL, &timeout);
  if (count < 0) {
    throw std::runtime_error("select() failed");
  }
  if (count == 0) {
    std::cerr << kTimeoutSec << "seconds passed without event" << std::endl;
  } else {
    readable_fds_ = toFdVector(readfds, maxfd);
    writable_fds_ = toFdVector(writefds, maxfd);
  }
}

void Selector::setFdset(fd_set *readfds, fd_set *writefds,
                        const ServerSocketMap &serv_socks,
                        const ClientSocketMap &clnt_socks) {
  FD_ZERO(readfds);
  FD_ZERO(writefds);
  for (ServerSocketMap::const_iterator it = serv_socks.begin();
       it != serv_socks.end(); it++) {
    FD_SET(it->first, readfds);
  }
  for (ClientSocketMap::const_iterator it = clnt_socks.begin();
       it != clnt_socks.end(); it++) {
    if (it->second->isWaitingForRequest()) {
      FD_SET(it->first, readfds);
    } else if (it->second->canResponse()) {
      FD_SET(it->first, writefds);
    } else {
      throw std::runtime_error("seFdset: unexpected clnt socket state");
    }
  }
}

int Selector::calcMaxFd(const ServerSocketMap &serv_socks,
                        const ClientSocketMap &clnt_socks) {
  int serv_maxfd = (serv_socks.empty()) ? -1 : serv_socks.rbegin()->first;
  int clnt_maxfd = (clnt_socks.empty()) ? -1 : clnt_socks.rbegin()->first;
  return std::max(serv_maxfd, clnt_maxfd);
}

FdVector Selector::toFdVector(const fd_set &fdset, int maxfd) {
  FdVector ret;
  for (int i = 0; i <= maxfd; i++) {
    if (FD_ISSET(i, &fdset)) {
      ret.push_back(i);
    }
  }
  return ret;
}

void Selector::showInfo(int maxfd, const ServerSocketMap &serv_socks,
                        const ClientSocketMap &clnt_socks) {
  std::cerr << "*** Selecter Target Info ***" << std::endl;
  std::cerr << "readfd  : ";
  for (ServerSocketMap::const_iterator it = serv_socks.begin();
       it != serv_socks.end(); it++) {
    std::cerr << it->first << ", ";
  }
  for (ClientSocketMap::const_iterator it = clnt_socks.begin();
       it != clnt_socks.end(); it++) {
    if (it->second->isWaitingForRequest()) {
      std::cerr << it->first << ", ";
    }
  }
  std::cerr << std::endl;
  std::cerr << "writefd : ";
  for (ClientSocketMap::const_iterator it = clnt_socks.begin();
       it != clnt_socks.end(); it++) {
    if (it->second->canResponse()) {
      std::cerr << it->first << ", ";
    }
  }
  std::cerr << std::endl;
  std::cerr << "maxfd   : " << maxfd << std::endl;
  std::cerr << "****************************" << std::endl;
  std::cerr << std::endl;
}
