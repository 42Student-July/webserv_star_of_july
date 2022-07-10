#ifndef SRC_UTILS_HPP_
#define SRC_UTILS_HPP_

#include "ASocket.hpp"
#include "Connection.hpp"
#include "ServerSocket.hpp"

namespace utils {
bool isServerSocket(ASocket *socket);
}  // namespace utils

#endif  // SRC_UTILS_HPP_
