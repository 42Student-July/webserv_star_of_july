#ifndef SRC_SELECTOR_HPP_
#define SRC_SELECTOR_HPP_

#include <sys/select.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "ASocket.hpp"
#include "ClientSocket.hpp"
#include "ServerSocket.hpp"
#include "type.hpp"
#include "utils.hpp"

// 責務：ソケットの集合を監視し、IOの準備ができたソケットの集合を返す
// selectをラップしているクラス
class Selector {
 public:
  Selector();
  ~Selector();

  void select(const ServerSocketMap &server_sock_map_,
              const ClientSocketMap &client_sock_map_);
  const FdVector readFds() const;
  const FdVector writeFds() const;

 private:
  static const int kTimeoutSec = 10;

  Selector(const Selector &other);
  Selector &operator=(const Selector &other);

  void clear();
  void storeTargetfds(const ServerSocketMap &server_sock_map_,
                      const ClientSocketMap &client_sock_map_);
  fd_set prepareReadfds();
  fd_set prepareWritefds();
  int calcMaxFd();
  FdVector toFdVector(const fd_set &fdset, int maxfd);
  void showInfo(int maxfd);

  FdVector target_readfds_;
  FdVector target_writefds_;
  FdVector readfds_;
  FdVector writefds_;
};

#endif  // SRC_SELECTOR_HPP_
