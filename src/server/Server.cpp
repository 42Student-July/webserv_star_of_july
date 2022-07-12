#include "Server.hpp"

Server::Server(const std::vector<ServerConfig> &serverconfigs) {
  for (std::vector<ServerConfig>::const_iterator it = serverconfigs.cbegin();
       it != serverconfigs.cend(); it++) {
    ServerSocket *serv_sock = new ServerSocket(*it);
    fd2socket_[serv_sock->getFd()] = serv_sock;
  }
}

Server::~Server() {}

void Server::run() {
  std::cerr << YELLOW "run server" RESET << std::endl;
  for (;;) {
    selector_.select(fd2socket_);
    SocketMap ready = selector_.getReadySockets();
    handleSockets(ready);
    destroyConnectionSockets();
  }
}

void Server::handleSockets(const Selector::SocketMap &sockets) {
  for (Selector::SocketMap::const_iterator it = sockets.begin();
       it != sockets.end(); it++) {
    if (utils::isServerSocket(it->second)) {
      handleServerSocket(dynamic_cast<ServerSocket *>(it->second));
    } else {
      handleConnectionSocket(dynamic_cast<ConnectionSocket *>(it->second));
    }
  }
}

void Server::handleServerSocket(const ServerSocket *socket) {
  ConnectionSocket *new_ConnectionSocket = socket->createConnectionSocket();

  fd2socket_[new_ConnectionSocket->getFd()] = new_ConnectionSocket;
}

void Server::handleConnectionSocket(ConnectionSocket *socket) {
  socket->handleCommunication();
}

// 通信を終えたConnectionSocketSocketを破棄する
void Server::destroyConnectionSockets() {
  for (SocketMap::const_iterator it = fd2socket_.begin();
       it != fd2socket_.cend(); it++) {
    if (!utils::isServerSocket(it->second)) {
      ConnectionSocket::State state =
          dynamic_cast<ConnectionSocket *>(it->second)->getState();
      if (state == ConnectionSocket::CLOSE) {
        delete it->second;
        fd2socket_.erase(it);
      }
    }
  }
}
