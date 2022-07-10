#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <iostream>

#include "ASocket.hpp"
#include "Connection.hpp"
#include "Selector.hpp"

class ServerSocket : public ASocket {
public:
  ServerSocket();
  ~ServerSocket();

  // void communicateWithClient(Selector *observer);
  Connection *createConnection() const;

private:
  static const int kServerPort = 8000;
  static const int kMaxPendig = 5;

  ServerSocket(const ServerSocket &other);
  ServerSocket &operator=(const ServerSocket &other);
};

#endif /* SERVERSOCKET_HPP */
