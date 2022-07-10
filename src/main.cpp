#include <iostream>

#include "ConnectionSocket.hpp"
#include "Server.hpp"
#include "ServerSocket.hpp"
#include "color.hpp"

int main() {
  std::cout << YELLOW "start program" RESET << std::endl;
  Server server;
  server.run();
}
