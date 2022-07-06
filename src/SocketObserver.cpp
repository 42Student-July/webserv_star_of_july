#include "SocketObserver.hpp"

SocketObserver::SocketObserver() {}

SocketObserver::~SocketObserver() {}

void SocketObserver::run() {
  for (;;) {
    update();
    showTarget();
    observe();
  }
}

void SocketObserver::addTarget(ASocket *socket, ObserveKind kind) {
  UpdateReservation rsv = {socket, kind, false};

  watch_reservation_.push_back(rsv);
}

void SocketObserver::delTarget(ASocket *socket, ObserveKind kind) {
  UpdateReservation rsv = {socket, kind, true};

  unwatch_reservation_.push_back(rsv);
}

void SocketObserver::modTarget(ASocket *socket, ObserveKind current,
                               ObserveKind next) {
  UpdateReservation unwatch_rsv = {socket, current, false};
  UpdateReservation watch_rsv = {socket, next, false};

  unwatch_reservation_.push_back(unwatch_rsv);
  watch_reservation_.push_back(watch_rsv);
}

void SocketObserver::observe() {
  int maxfd = getMaxFd();
  fd_set readfds = toFdset(read_sockets_);
  fd_set writefds = toFdset(write_sockets_);
  struct timeval timeout = {kTimeoutSec, 0};

  int event_count = select(maxfd + 1, &readfds, &writefds, NULL, &timeout);
  if (event_count < 0) {
    throw std::runtime_error("select() failed");
  } else if (event_count == 0) {
    std::cerr << "Time out, you had tea break?" << std::endl;
  } else {
    notifySockets(read_sockets_, readfds);
    notifySockets(write_sockets_, writefds);
  }
}

void SocketObserver::update() {
  for (std::vector<UpdateReservation>::iterator it =
           unwatch_reservation_.begin();
       it != unwatch_reservation_.end(); it++) {
    unwatch(*it);
  }
  for (std::vector<UpdateReservation>::iterator it = watch_reservation_.begin();
       it != watch_reservation_.end(); it++) {
    watch(*it);
  }
  unwatch_reservation_.clear();
  watch_reservation_.clear();
}

void SocketObserver::showTarget() {
  std::cerr << "###Read Target  : ";
  for (SocketMap::const_iterator it = read_sockets_.begin();
       it != read_sockets_.cend(); it++) {
    std::cerr << it->first << ", ";
  }
  std::cerr << std::endl;
  std::cerr << "###Write Target : ";
  for (SocketMap::const_iterator it = write_sockets_.begin();
       it != write_sockets_.cend(); it++) {
    std::cerr << it->first << ", ";
  }
  std::cerr << std::endl << std::endl;
}

void SocketObserver::unwatch(const UpdateReservation &reservation) {
  int index_fd = reservation.socket->getFd();

  if (reservation.kind == READ) {
    read_sockets_.erase(index_fd);
  } else if (reservation.kind == WRITE) {
    write_sockets_.erase(index_fd);
  }
  if (reservation.should_destroy) {
    delete reservation.socket;
  }
}

void SocketObserver::watch(const UpdateReservation &reservation) {
  int index_fd = reservation.socket->getFd();

  if (reservation.kind == READ) {
    read_sockets_[index_fd] = reservation.socket;
  } else if (reservation.kind == WRITE) {
    write_sockets_[index_fd] = reservation.socket;
  }
}

int SocketObserver::getMaxFd() {
  int maxfd = -1;
  int maxfd_read = -1;
  int maxfd_write = -1;

  if (!read_sockets_.empty()) {
    maxfd_read = read_sockets_.rbegin()->first;
  }
  if (!write_sockets_.empty()) {
    maxfd_write = write_sockets_.rbegin()->first;
  }
  maxfd = (maxfd_read > maxfd_write) ? maxfd_read : maxfd_write;

  return maxfd;
}

fd_set SocketObserver::toFdset(const SocketMap &sockets) {
  fd_set ret;

  FD_ZERO(&ret);
  for (SocketMap::const_iterator it = sockets.cbegin(); it != sockets.cend();
       it++) {
    FD_SET(it->first, &ret);
  }
  return ret;
}

void SocketObserver::notifySockets(const SocketMap &sockets,
                                   const fd_set &fdsets) {
  for (SocketMap::const_iterator it = sockets.cbegin(); it != sockets.cend();
       it++) {
    if (FD_ISSET(it->first, &fdsets))
      it->second->communicateWithClient(this);
  }
}
