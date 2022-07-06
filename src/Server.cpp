#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::run() {
  ServerSocket serv_sock;

  SocketObserver observer;
  observer.addTarget(&serv_sock, SocketObserver::READ);
  observer.run();
}
