#ifndef SRC_ASOCKET_HPP_
#define SRC_ASOCKET_HPP_

#include <map>

#include "ServerConfig.hpp"

class ASocket {
 public:
  ASocket(int fd, const ServerConfig &serverconfig);
  virtual ~ASocket();

  int getFd() const;

 protected:
  int fd_;
  ServerConfig serverconfig_;

 private:
  ASocket(const ASocket &other);
  ASocket &operator=(const ASocket &other);
};

#endif  // SRC_ASOCKET_HPP_
