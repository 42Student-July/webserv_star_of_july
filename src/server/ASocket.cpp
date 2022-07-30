#include "ASocket.hpp"

ASocket::ASocket(const ServerConfig &serverconfig)
    : serverconfig_(serverconfig) {}

ASocket::ASocket(int fd, const ServerConfig &serverconfig)
    : fd_(fd), serverconfig_(serverconfig) {}

ASocket::~ASocket() {}

int ASocket::getFd() const { return fd_; }
