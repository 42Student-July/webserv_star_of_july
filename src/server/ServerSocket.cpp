#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const ServerConfig &serverconfig)
    : ASocket(serverconfig) {
  createEndpoint();
  setLocalAddress();
  listen();
}

ServerSocket::~ServerSocket() {}

ClientSocket *ServerSocket::acceptConnection() const {
  int new_socket = accept(fd_, NULL, NULL);
  if (new_socket < 0) {
    throw std::runtime_error("accept failed()");
  }
  setNonBlocking(new_socket);
  return new ClientSocket(new_socket, serverconfig_);
}

void ServerSocket::createEndpoint() {
  fd_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd_ < 0) {
    throw std::runtime_error("socket() failed");
  }
  int opt = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&opt),
                 sizeof(int)) < 0) {
    throw std::runtime_error("setsockopt() failed");
  }
  setNonBlocking(fd_);
}

void ServerSocket::setNonBlocking(int fd) {
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
    throw std::runtime_error("fcntl() failed");
  }
}

void ServerSocket::setLocalAddress() {
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(serverconfig_.port);
  if (bind(fd_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    throw std::runtime_error("bind() failed");
  }
}

void ServerSocket::listen() {
  if (::listen(fd_, SOMAXCONN) < 0) {
    throw std::runtime_error("listen() failed");
  }
}
