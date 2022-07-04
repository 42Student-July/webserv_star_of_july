#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <iostream>

#include "ASocket.hpp"
#include "Connection.hpp"
#include "Observer.hpp"

class ServerSocket : public ASocket {
 public:
  ServerSocket();
  ~ServerSocket();

  void notifyFdEvent(Observer* observer, std::map<int, ASocket*>* fd2socket);

 private:
  static const int kServerPort = 5000;
  static const int kMaxPendig = 5;

  ServerSocket(const ServerSocket& other);
  ServerSocket& operator=(const ServerSocket& other);

  Connection* createConnection(Observer* observer);
};

#endif /* SERVERSOCKET_HPP */
