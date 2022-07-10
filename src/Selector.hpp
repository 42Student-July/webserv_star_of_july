#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <map>
#include <set>
#include <vector>

#include "ASocket.hpp"
#include "Connection.hpp"
#include "ServerSocket.hpp"
#include "utils.hpp"
#include <iostream>
#include <sys/select.h>

class Selector {
public:
  typedef std::map<int, ASocket *> SocketMap;

  Selector();
  ~Selector();

  int select(const SocketMap &fd2socket);
  SocketMap &getReadySockets();
  SocketMap &getReadyReadSockets();  // 現状使ってない
  SocketMap &getReadyWriteSockets(); // 現状使ってない

private:
  static const int kTimeoutSec = 10;

  Selector(const Selector &other);
  Selector &operator=(const Selector &other);

  void clear();
  void init(const SocketMap &fd2socket);
  void showTarget();
  int getMaxFd();
  fd_set toFdset(const SocketMap &sockets);
  SocketMap toSocketMap(const fd_set &fdset, const SocketMap &target_fds);
  void addTarget(Connection *sockets);

  SocketMap ready_all_;
  SocketMap ready_read_;
  SocketMap ready_write_;
  SocketMap target_read_;
  SocketMap target_write_;
};

#endif /* SELECTOR */
