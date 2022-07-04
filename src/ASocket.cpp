#include "ASocket.hpp"

ASocket::ASocket(int fd) : fd_(fd) {}

ASocket::~ASocket() {}

int ASocket::getFd() const { return fd_; }
