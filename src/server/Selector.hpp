#ifndef SRC_SELECTOR_HPP_
#define SRC_SELECTOR_HPP_

#include <sys/select.h>

#include <algorithm>
#include <iostream>

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

  void select(const ServerSocketMap &serv_socks,
              const ClientSocketMap &clnt_socks);
  const FdVector readableFds() const;
  const FdVector writableFds() const;

 private:
  static const int kTimeoutSec = 5;

  Selector(const Selector &other);
  Selector &operator=(const Selector &other);
  void setFdset(fd_set *readfds, fd_set *writefds,
                const ServerSocketMap &serv_socks,
                const ClientSocketMap &clnt_socks);
  int calcMaxFd(const ServerSocketMap &serv_socks,
                const ClientSocketMap &clnt_socks);
  FdVector toFdVector(const fd_set &fdset, int maxfd);
  void showInfo(int maxfd, const ServerSocketMap &serv_socks,
                const ClientSocketMap &clnt_socks);

  FdVector readable_fds_;
  FdVector writable_fds_;
};

#endif  // SRC_SELECTOR_HPP_
