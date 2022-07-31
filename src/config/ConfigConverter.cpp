#include "ConfigConverter.hpp"

ConfigConverter::ConfigConverter() {}

ConfigConverter::~ConfigConverter() {}

#include <iostream>
ConfigDTO* ConfigConverter::toDTO(ServerConfig conf) {
  ConfigDTO* dto = new ConfigDTO;
  //一旦0番目でコンバートする
  // vectorが空の時セグフォしたので応急処置
  if (!conf.server.empty()) {
    dto->server = conf.server[0];
    // std::cerr << "conf.server.size()" << conf.server.size() << std::endl;
    // std::cerr << "conf.server.empty()" << conf.server.empty() << std::endl;
  }
  // std::cerr << "dto->server" << dto->server << std::endl;
  // dto->server = conf.server[0];
  dto->error_pages = conf.error_pages;
  dto->host = conf.host;
  dto->root = conf.root;
  std::stringstream ss;
  ss << conf.port;
  dto->port = ss.str();
  dto->locations = conf.locations;

  return dto;
}
