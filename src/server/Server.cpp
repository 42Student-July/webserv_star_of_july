#include "Server.hpp"

Server::Server(const std::vector<ServerConfig> &serverconfigs) {
  for (std::vector<ServerConfig>::const_iterator it = serverconfigs.begin();
       it != serverconfigs.end(); it++) {
    ServerSocket *serv_sock = new ServerSocket(*it);
    server_sock_map_[serv_sock->getFd()] = serv_sock;
  }
}

Server::~Server() {}

void Server::run() {
  std::cerr << YELLOW "run server" RESET << std::endl;
  for (;;) {
    Selector sel;
    // selector_.select(server_sock_map_, client_sock_map_);
    sel.select(server_sock_map_, client_sock_map_);
    handleReadEvent(sel.readableFds());
    handleWriteEvent(sel.writableFds());
    destroyClient();
  }
}

bool Server::isServerSocketFd(int fd) {
  return server_sock_map_.find(fd) != server_sock_map_.end();
}

void Server::handleReadEvent(const FdVector &readyfd) {
  for (FdVector::const_iterator it = readyfd.begin(); it != readyfd.end();
       it++) {
    if (isServerSocketFd(*it)) {
      ClientSocket *new_clnt = server_sock_map_[*it]->acceptConnection();
      client_sock_map_[new_clnt->getFd()] = new_clnt;
    } else {
      client_sock_map_[*it]->handleReadEvent();
    }
  }
}

void Server::handleWriteEvent(const FdVector &readyfd) {
  for (FdVector::const_iterator it = readyfd.begin(); it != readyfd.end();
       it++) {
    client_sock_map_[*it]->handleWriteEvent();
  }
}

// 通信を終えたConnectionSocketを破棄する
void Server::destroyClient() {
  ClientSocketMap::iterator it = client_sock_map_.begin();
  while (it != client_sock_map_.end()) {
    ClientSocket::State state = it->second->getState();
    if (state == ClientSocket::CLOSE) {
      ClientSocketMap::iterator tmp_it = it;
      it++;
      delete tmp_it->second;
      client_sock_map_.erase(tmp_it);
    } else {
      it++;
    }
  }
}
