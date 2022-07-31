#ifndef SRC_ASOCKET_HPP_
#define SRC_ASOCKET_HPP_

#include <map>

#include "ServerConfig.hpp"

class ASocket {
 public:
  ASocket(size_t port, int fd = -1);
  virtual ~ASocket();

  int getFd() const;

 protected:
  size_t port_;
  int fd_;

 private:
  ASocket(const ASocket &other);
  ASocket &operator=(const ASocket &other);
};

#endif  // SRC_ASOCKET_HPP_
