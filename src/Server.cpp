#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::run() {
  ServerSocket serv_sock;
  std::map<int, ASocket *> fd2socket;
  fd2socket[serv_sock.getFd()] = &serv_sock;

  EventLoop eventloop(fd2socket);
  eventloop.loop();
}
