#ifndef SRC_SERVER_HPP_
#define SRC_SERVER_HPP_

#include <map>
#include <vector>

#include "ClientSocket.hpp"
#include "Selector.hpp"
#include "ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "color.hpp"
#include "utils.hpp"

class Server {
 public:
  typedef std::map<int, ASocket *> SocketMap;

  explicit Server(const std::vector<ServerConfig> &serverconfigs);
  ~Server();

  void run();

 private:
  Server();
  Server(const Server &other);
  Server &operator=(const Server &other);

  void handleSockets(const SocketMap &sockets);
  void handleServerSocket(const ServerSocket *socket);
  static void handleConnectionSocket(ClientSocket *socket);
  void destroyConnectionSockets();

  Selector selector_;
  SocketMap fd2socket_;
};

#endif  // SRC_SERVER_HPP_
