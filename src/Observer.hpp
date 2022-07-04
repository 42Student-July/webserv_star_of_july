#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <sys/select.h>
#include <time.h>

#include <iostream>
#include <map>
#include <set>

// 循環防止用。インクルードは下でしてる
class ASocket;

class Observer {
 public:
  Observer();
  ~Observer();

  void init();
  // void updateSockets(std::map<int, ASocket*>* fd2socket);
  void notifyFdEvent();
  void addTargetReadFd(int fd);
  void addTargetWriteFd(int fd);
  void delTargetReadFd(int fd);
  void delTargetWriteFd(int fd);
  const std::set<int>& getReadyFds() const;
  const std::set<int>& getTargetFds() const;
  void showInfo() const;

 private:
  static const int kTimeoutSec = 5;

  Observer(const Observer& other);
  Observer& operator=(const Observer& other);

  void printfdset(fd_set fds) const;
  std::set<int> buildReadyFds(fd_set readfds, fd_set writefds) const;

  int maxfd_;
  int evnet_cnt_;
  fd_set readfds_;
  fd_set writefds_;
  struct timeval timeout_;

  std::set<int> readyfds_;
  std::set<int> targetfds_;
  ;
};

#include "ASocket.hpp"
#endif /* OBSERVER_HPP */
