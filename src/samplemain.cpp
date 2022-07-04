#include <iostream>

#include "Connection.hpp"
#include "Server.hpp"
#include "ServerSocket.hpp"
#include "color.hpp"

int main() {
  std::cout << YELLOW "start program" RESET << std::endl;
  Server server;
  server.run();
}
