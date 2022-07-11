#include <iostream>

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "color.hpp"

int main(int argc, const char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./webserv  <configfile>" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    ConfigParser config_parser(argv[1]);
    Server server(config_parser.getServerConfigs());

    server.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
