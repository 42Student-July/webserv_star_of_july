#include "utils.hpp"

bool utils::isServerSocket(ASocket *socket) {
  if (dynamic_cast<ServerSocket *>(socket)) {
    return true;
  } else {
    return false;
  }
}
