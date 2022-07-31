#ifndef SRC_SERVERSOCKET_HPP_
#define SRC_SERVERSOCKET_HPP_

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <iostream>

#include "ASocket.hpp"
#include "ClientSocket.hpp"
#include "ServerConfig.hpp"

class ServerSocket : public ASocket {
 public:
  explicit ServerSocket(size_t port);
  ~ServerSocket();

  ClientSocket *acceptConnection() const;

 private:
  ServerSocket();
  ServerSocket(const ServerSocket &other);
  ServerSocket &operator=(const ServerSocket &other);
  void createEndpoint();
  void setLocalAddress();
  void listen();
  static void setNonBlocking(int fd);
};

#endif  // SRC_SERVERSOCKET_HPP_
