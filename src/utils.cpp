#include "utils.hpp"

bool utils::isServerSocket(ASocket *socket) {
  return dynamic_cast<ServerSocket *>(socket) != NULL;
}
