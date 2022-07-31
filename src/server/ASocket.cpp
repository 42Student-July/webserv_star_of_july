#include "ASocket.hpp"

ASocket::ASocket(size_t port, int fd) : port_(port), fd_(fd) {}

ASocket::~ASocket() {}

int ASocket::getFd() const { return fd_; }
