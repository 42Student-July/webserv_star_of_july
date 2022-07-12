#ifndef SRC_SERVERSOCKET_HPP_
#define SRC_SERVERSOCKET_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <iostream>

#include "ASocket.hpp"
#include "ConnectionSocket.hpp"
#include "ServerConfig.hpp"

class ServerSocket : public ASocket {
 public:
  explicit ServerSocket(const ServerConfig &serverconfig);
  ~ServerSocket();

  ConnectionSocket *createConnectionSocket() const;

 private:
  static const int kServerPort = 8000;
  static const int kMaxPendig = 5;

  ServerSocket();
  ServerSocket(const ServerSocket &other);
  ServerSocket &operator=(const ServerSocket &other);
};

#endif  // SRC_SERVERSOCKET_HPP_
