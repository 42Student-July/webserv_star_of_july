#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ServerConfig.hpp"

#define BUFFER_SIZE 128

// コンストラクタでparseを実行
// getServerConfigsで必要な情報(ServerConfigのベクター)を外部へ渡す
class ConfigParser {
 public:
  ConfigParser();
  explicit ConfigParser(std::string file);
  virtual ~ConfigParser();

  std::vector<ServerConfig> getServerConfigs() const;

 private:
  std::vector<ServerConfig> serverconfigs_;

  // locationのparse
  void parseLocation(LocationConfig &location,
                     std::vector<std::string>::iterator &it,
                     std::vector<std::string>::iterator &ite);
  void parseLocationCGIPath(LocationConfig &location,
                            std::vector<std::string>::iterator &it);
  void parseLocationAutoindexes(LocationConfig &location,
                                std::vector<std::string>::iterator &it);
  void parseLocationIndexes(LocationConfig &location,
                            std::vector<std::string>::iterator &it);
  void parseLocationRoot(LocationConfig &location,
                         std::vector<std::string>::iterator &it);
  void parseLocationAllowedMethods(LocationConfig &location,
                                   std::vector<std::string>::iterator &it);
  void parseLocationRedirect(LocationConfig &location,
								std::vector<std::string>::iterator &it);

  // serverのparse
  void parseServer(ServerConfig &server, std::vector<std::string>::iterator &it,
                   std::vector<std::string>::iterator &ite);
  void parseClientBodySizeLimit(ServerConfig &server,
                                std::vector<std::string>::iterator &it);
  void parseErrorPages(ServerConfig &server,
                       std::vector<std::string>::iterator &it);
  void parseRoot(ServerConfig &server, std::vector<std::string>::iterator &it);
  void parseServerName(ServerConfig &server,
                       std::vector<std::string>::iterator &it);
  void parseListen(ServerConfig &server,
                   std::vector<std::string>::iterator &it);
  void parseTokens(std::vector<std::string> tokens);

  // token化のためのutils
  std::string readFile(std::string const file);
  std::vector<std::string> isspaceSplit(std::string const str);

  static int ft_stoi(std::string const &str);

  // 使わない関数
  ConfigParser(ConfigParser const &other);
  ConfigParser &operator=(ConfigParser const &other);
};

#endif
