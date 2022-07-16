#ifndef CONFIGDTO_HPP
#define CONFIGDTO_HPP

#include <map>
#include <string>
#include <vector>

#include "../config/ServerConfig.hpp"

struct ConfigDTO {
  std::string port;
  std::string host;
  std::map<int, std::string> error_pages;
  std::string root;
  std::string server;
  std::vector<LocationConfig> locations;
};

#endif
