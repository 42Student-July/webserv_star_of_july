#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include <map>

class ASocket {
public:
  ASocket(int fd);
  virtual ~ASocket();
  // virtual void communicateWithClient(Selector *observer) = 0;
  int getFd() const;

protected:
  int fd_;

private:
  ASocket(const ASocket &other);
  ASocket &operator=(const ASocket &other);
};

#endif /* ASOCKET_HPP */
