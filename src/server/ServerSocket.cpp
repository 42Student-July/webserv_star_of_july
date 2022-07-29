#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const ServerConfig &serverconfig)
    : ASocket(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP), serverconfig) {
  struct sockaddr_in server_addr;

  if (fd_ < 0) {
    throw std::runtime_error("socket() failed");
  }
  if (fcntl(fd_, F_SETFL, O_NONBLOCK) < 0) {
    throw std::runtime_error("fcntl() failed");
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(serverconfig.port);

  int opt = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&opt),
                 sizeof(int)) < 0) {
    throw std::runtime_error("setsockopt() failed");
  }

  if (bind(fd_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    throw std::runtime_error("bind() failed");
  }

  if (listen(fd_, kMaxPendig) < 0) {
    throw std::runtime_error("listen() failed");
  }
}

ServerSocket::~ServerSocket() {}

ConnectionSocket *ServerSocket::createConnectionSocket() const {
  int new_socket = accept(fd_, NULL, NULL);

  if (new_socket < 0) {
    throw std::runtime_error("accept failed()");
  }
  if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0) {
    throw std::runtime_error("fcntl() failed");
  }
  ConnectionSocket *new_ConnectionSocket =
      new ConnectionSocket(new_socket, serverconfig_);
  // クライアントの数のバリデーション
  return new_ConnectionSocket;
}
