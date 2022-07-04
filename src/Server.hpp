#ifndef HTTPSERVER_SRC_SERVER_HPP_
#define HTTPSERVER_SRC_SERVER_HPP_

#include <map>

#include "EventLoop.hpp"
#include "ServerSocket.hpp"

class Server {
 public:
  Server();
  ~Server();

  void run();

 private:
  // not copy
  Server(const Server& other);
  Server& operator=(const Server& other);
};

#endif  // HTTPSERVER_SRC_SERVER_HPP_
