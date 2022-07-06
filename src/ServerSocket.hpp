#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <iostream>

#include "ASocket.hpp"
#include "Connection.hpp"
#include "SocketObserver.hpp"

class ServerSocket : public ASocket {
public:
  ServerSocket();
  ~ServerSocket();

  void communicateWithClient(SocketObserver *observer);

private:
  static const int kServerPort = 5000;
  static const int kMaxPendig = 5;

  ServerSocket(const ServerSocket &other);
  ServerSocket &operator=(const ServerSocket &other);

  Connection *createConnection();
};

#endif /* SERVERSOCKET_HPP */
