#ifndef SRC_SERVER_HPP_
#define SRC_SERVER_HPP_

#include <map>

#include "ConnectionSocket.hpp"
#include "Selector.hpp"
#include "ServerSocket.hpp"
#include "utils.hpp"

class Server {
 public:
  typedef std::map<int, ASocket *> SocketMap;

  Server();
  ~Server();

  void run();

 private:
  // not copy
  Server(const Server &other);
  Server &operator=(const Server &other);

  void handleSockets(const SocketMap &sockets);
  void handleServerSocket(const ServerSocket *socket);
  static void handleConnectionSocket(ConnectionSocket *socket);
  void destroyConnectionSockets();

  Selector selector_;
  SocketMap fd2socket_;
};

#endif  // SRC_SERVER_HPP_
