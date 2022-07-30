#include "Server.hpp"

Server::Server(const std::vector<ServerConfig> &serverconfigs) {
  for (std::vector<ServerConfig>::const_iterator it = serverconfigs.begin();
       it != serverconfigs.end(); it++) {
    ServerSocket *serv_sock = new ServerSocket(*it);
    serv_socks_[serv_sock->getFd()] = serv_sock;
  }
}

Server::~Server() {}

void Server::run() {
  std::cerr << YELLOW "run server" RESET << std::endl;
  for (;;) {
    Selector sel;
    sel.select(serv_socks_, clnt_socks_);
    handleReadEvent(sel.readableFds());
    handleWriteEvent(sel.writableFds());
    destroyClient();
  }
}

bool Server::isServerSocketFd(int fd) {
  return serv_socks_.find(fd) != serv_socks_.end();
}

void Server::handleReadEvent(const FdVector &readable_fds) {
  for (FdVector::const_iterator it = readable_fds.begin();
       it != readable_fds.end(); it++) {
    if (isServerSocketFd(*it)) {
      ClientSocket *new_clnt = serv_socks_[*it]->acceptConnection();
      clnt_socks_[new_clnt->getFd()] = new_clnt;
    } else {
      clnt_socks_[*it]->handleReadEvent();
    }
  }
}

void Server::handleWriteEvent(const FdVector &writable_fds) {
  for (FdVector::const_iterator it = writable_fds.begin();
       it != writable_fds.end(); it++) {
    clnt_socks_[*it]->handleWriteEvent();
  }
}

// 通信を終えたConnectionSocketを破棄する
void Server::destroyClient() {
  ClientSocketMap::iterator it = clnt_socks_.begin();
  while (it != clnt_socks_.end()) {
    ClientSocket::State state = it->second->getState();
    if (state == ClientSocket::CLOSE) {
      ClientSocketMap::iterator tmp_it = it;
      it++;
      delete tmp_it->second;
      clnt_socks_.erase(tmp_it);
    } else {
      it++;
    }
  }
}
