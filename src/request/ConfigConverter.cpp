#include "ConfigConverter.hpp"

ConfigConverter::ConfigConverter()
{
}

ConfigConverter::~ConfigConverter()
{
}

ConfigDTO * ConfigConverter::toDTO(ServerConfig conf)
{
  ConfigDTO* dto = new ConfigDTO;
  //一旦0番目でコンバートする
  dto->server = conf.server[0];
  dto->error_pages = conf.error_pages;
  dto->host = conf.host;
  dto->root = conf.root;
  std::stringstream ss;
  ss << conf.port;
  dto->port = ss.str();
  dto->locations = conf.locations;

  return dto;
}
