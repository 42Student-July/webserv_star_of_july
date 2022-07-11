#ifndef SRC_SELECTOR_HPP_
#define SRC_SELECTOR_HPP_

#include <sys/select.h>

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "ASocket.hpp"
#include "ConnectionSocket.hpp"
#include "ServerSocket.hpp"
#include "utils.hpp"

// 責務：ソケットの集合を監視し、IOの準備ができたソケットの集合を返す
// selectをラップしているクラス
class Selector {
 public:
  typedef std::map<int, ASocket *> SocketMap;

  Selector();
  ~Selector();

  int select(const SocketMap &fd2socket);
  SocketMap &getReadySockets();
  SocketMap &getReadyReadSockets();   // 現状使ってない
  SocketMap &getReadyWriteSockets();  // 現状使ってない

 private:
  static const int kTimeoutSec = 10;

  Selector(const Selector &other);
  Selector &operator=(const Selector &other);

  void clear();
  void init(const SocketMap &fd2socket);
  void showTarget();
  int getMaxFd();
  void addTarget(ConnectionSocket *sockets);

  static fd_set toFdset(const SocketMap &sockets);
  static SocketMap toSocketMap(const fd_set &fdset,
                               const SocketMap &target_fds);

  SocketMap ready_all_;
  SocketMap ready_read_;
  SocketMap ready_write_;
  SocketMap target_read_;
  SocketMap target_write_;
};

#endif  // SRC_SELECTOR_HPP_
