#include "Server.hpp"

Server::Server(const std::vector<ServerConfig> &serverconfigs) {
  for (std::vector<ServerConfig>::const_iterator it = serverconfigs.begin();
       it != serverconfigs.end(); it++) {
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
      handleConnectionSocket(dynamic_cast<ClientSocket *>(it->second));
    }
  }
}

void Server::handleServerSocket(const ServerSocket *socket) {
  ClientSocket *new_ConnectionSocket = socket->acceptConnection();

  fd2socket_[new_ConnectionSocket->getFd()] = new_ConnectionSocket;
}

void Server::handleConnectionSocket(ClientSocket *socket) {
  socket->handleCommunication();
}

// 通信を終えたConnectionSocketSocketを破棄する
void Server::destroyConnectionSockets() {
  for (SocketMap::iterator it = fd2socket_.begin(); it != fd2socket_.end();
       it++) {
    if (!utils::isServerSocket(it->second)) {
      ClientSocket::State state =
          dynamic_cast<ClientSocket *>(it->second)->getState();
      if (state == ClientSocket::CLOSE) {
        SocketMap::iterator tmp_it = it;
        it++;
        delete tmp_it->second;
        fd2socket_.erase(tmp_it);
        it--;
      }
    }
  }
}
