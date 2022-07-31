#include <iostream>

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "color.hpp"
#include "response.h"

int main(int argc, const char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./webserv  <configfile>" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    ConfigParser parser(argv[1]);
    Server server(parser.getWebservConfig());

    server.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
