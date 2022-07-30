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
  explicit Server(const std::vector<ServerConfig> &serverconfigs);
  ~Server();

  void run();

 private:
  Server();
  Server(const Server &other);
  Server &operator=(const Server &other);

  void handleReadEvent(const std::vector<int> &readyfd);
  void handleWriteEvent(const std::vector<int> &writefd);
  void handleServerSocket(const ServerSocket *socket);
  static void handleConnectionSocket(ClientSocket *socket);
  bool isServerSocketFd(int fd);
  void destroyClient();

  Selector selector_;
  ServerSocketMap server_sock_map_;
  ClientSocketMap client_sock_map_;
};

#endif  // SRC_SERVER_HPP_
