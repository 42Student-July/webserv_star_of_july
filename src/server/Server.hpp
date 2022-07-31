#ifndef SRC_SERVER_HPP_
#define SRC_SERVER_HPP_

#include "ClientSocket.hpp"
#include "Selector.hpp"
#include "ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "WebservConfig.hpp"
#include "color.hpp"
#include "type.hpp"

class Server {
 public:
  explicit Server(const WebservConfig &config);
  ~Server();
  void run();

 private:
  Server();
  Server(const Server &other);
  Server &operator=(const Server &other);
  void handleReadEvent(const FdVector &readable_fds);
  void handleWriteEvent(const FdVector &writable_fds);
  void destroyClient();
  bool isServerSocketFd(int fd);

  ServerSocketMap serv_socks_;
  ClientSocketMap clnt_socks_;
  WebservConfig config_;
};

#endif  // SRC_SERVER_HPP_
