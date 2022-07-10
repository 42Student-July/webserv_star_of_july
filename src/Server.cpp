#include "Server.hpp"

Server::Server() {
  ServerSocket *serv_sock = new ServerSocket;

  fd2socket_[serv_sock->getFd()] = serv_sock;
}

Server::~Server() {}

void Server::run() {
  for (;;) {
    selector_.select(fd2socket_);
    SocketMap ready = selector_.getReadySockets();
    handleSockets(ready);
    destroyConnections();
  }
}

void Server::handleSockets(const Selector::SocketMap &sockets) {
  for (Selector::SocketMap::const_iterator it = sockets.begin();
       it != sockets.end(); it++) {
    if (utils::isServerSocket(it->second)) {
      handleServerSocket(dynamic_cast<ServerSocket *>(it->second));
    } else {
      handleConnection(dynamic_cast<Connection *>(it->second));
    }
  }
}

void Server::handleServerSocket(const ServerSocket *socket) {
  Connection *new_connection = socket->createConnection();

  fd2socket_[new_connection->getFd()] = new_connection;
}

void Server::handleConnection(Connection *socket) {
  socket->handleCommunication();
}

// 通信を終えたConnectionSocketを破棄する
void Server::destroyConnections() {
  for (SocketMap::const_iterator it = fd2socket_.begin();
       it != fd2socket_.cend(); it++) {
    if (!utils::isServerSocket(it->second)) {
      Connection::State state =
          dynamic_cast<Connection *>(it->second)->getState();
      if (state == Connection::CLOSE) {
        delete it->second;
        fd2socket_.erase(it);
      }
    }
  }
}
