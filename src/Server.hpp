#ifndef HTTPSERVER_SRC_SERVER_HPP_
#define HTTPSERVER_SRC_SERVER_HPP_

#include <map>

#include "Selector.hpp"
#include "ServerSocket.hpp"
#include "utils.hpp"

class Server {
public:
  typedef std::map<int, ASocket *> SocketMap;

  Server();
  ~Server();

  void run();
  // void init();
  void handleSockets(const SocketMap &sockets);

private:
  // not copy
  Server(const Server &other);
  Server &operator=(const Server &other);

  void handleServerSocket(const ServerSocket &socket);
  void handleConnection(Connection &socket);
  void closeSockets();

  Selector observer_;
  SocketMap fd2socket_;
};

#endif // HTTPSERVER_SRC_SERVER_HPP_
