#ifndef SOCKETOBSERVER
#define SOCKETOBSERVER

#include <map>
#include <set>
#include <vector>

#include "ASocket.hpp"
#include <iostream>

class SocketObserver {
public:
  // 名前が微妙
  enum ObserveKind {
    READ,
    WRITE,
    EXCEPTION,
    NONE,
  };
  SocketObserver();
  ~SocketObserver();

  void run();
  void addTarget(ASocket *socket, ObserveKind kind);
  void delTarget(ASocket *socket, ObserveKind kind);
  void modTarget(ASocket *socket, ObserveKind current, ObserveKind next);

private:
  typedef std::map<int, ASocket *> SocketMap;

  typedef struct UpdateReservation {
    ASocket *socket;
    ObserveKind kind;
    bool should_destroy;
  } UpdateReservation;

  static const int kTimeoutSec = 5;

  SocketObserver(const SocketObserver &other);
  SocketObserver &operator=(const SocketObserver &other);

  void observe();
  void update();
  void showTarget();
  void unwatch(const UpdateReservation &reservation);
  void watch(const UpdateReservation &reservation);
  int getMaxFd();
  fd_set toFdset(const SocketMap &sockets);
  void notifySockets(const SocketMap &sockets, const fd_set &fdsets);

  SocketMap read_sockets_;
  SocketMap write_sockets_;
  // std::map<int, ASocket *> exeption_sockets_; write_sockets_;
  std::vector<UpdateReservation> unwatch_reservation_;
  std::vector<UpdateReservation> watch_reservation_;
};

#endif /* SOCKETOBSERVER */
