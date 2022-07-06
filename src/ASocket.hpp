#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include <map>

class SocketObserver;

class ASocket {
public:
  ASocket(int fd);
  virtual ~ASocket();
  virtual void communicateWithClient(SocketObserver *observer) = 0;
  int getFd() const;

protected:
  int fd_;

private:
  ASocket(const ASocket &other);
  ASocket &operator=(const ASocket &other);
};

#include "SocketObserver.hpp"

#endif /* ASOCKET_HPP */
