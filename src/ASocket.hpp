#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include <map>

#include "Observer.hpp"

class ASocket {
 public:
  ASocket(int fd);
  virtual ~ASocket();
  virtual void notifyFdEvent(Observer* observer,
                             std::map<int, ASocket*>* fd2socket) = 0;
  int getFd() const;

 protected:
  int fd_;

 private:
  ASocket(const ASocket& other);
  ASocket& operator=(const ASocket& other);
};

#endif /* ASOCKET_HPP */
