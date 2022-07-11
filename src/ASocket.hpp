#ifndef SRC_ASOCKET_HPP_
#define SRC_ASOCKET_HPP_

#include <map>

class ASocket {
 public:
  explicit ASocket(int fd);
  virtual ~ASocket();

  int getFd() const;

 protected:
  int fd_;

 private:
  ASocket(const ASocket &other);
  ASocket &operator=(const ASocket &other);
};

#endif  // SRC_ASOCKET_HPP_
