#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../utility/utility.hpp"
#include "ServerConfig.hpp"
#include "WebservConfig.hpp"

#define BUFFER_SIZE 128

// コンストラクタでparseを実行
// getServerConfigsで必要な情報(ServerConfigのベクター)を外部へ渡す
class ConfigParser {
 public:
  ConfigParser();
  explicit ConfigParser(std::string file);
  virtual ~ConfigParser();

  std::vector<ServerConfig> getServerConfigs() const;
  const WebservConfig &getWebservConfig() const;

 private:
  std::vector<ServerConfig> serverconfigs_;
  WebservConfig webserv_config_;

  static const unsigned int BIT_FLAG_LISTEN;      // 0000 0000 0000 0001
  static const unsigned int BIT_FLAG_ROOT;        // 0000 0000 0000 0010
  static const unsigned int BIT_FLAG_BODY_LIMIT;  // 0000 0000 0000 0100
  static const unsigned int BIT_FLAG_LOC_ROOT;    // 0000 0000 0001 0000
  static const unsigned int BIT_FLAG_AUTOINDEX;   // 0000 0000 0010 0000

  static const std::vector<std::string> VALID_METHODS;
  static std::vector<std::string> setValidMethods();

  // locationのparse
  void parseLocation(LocationConfig &location,
                     std::vector<std::string>::iterator &it,
                     std::vector<std::string>::iterator &ite);
  void parseLocationCGIExtension(LocationConfig &location,
                                 std::vector<std::string>::iterator &it);
  void parseLocationAutoindexes(LocationConfig &location,
                                std::vector<std::string>::iterator &it,
                                unsigned int &l_exist_flag);
  void parseLocationIndexes(LocationConfig &location,
                            std::vector<std::string>::iterator &it);
  void parseLocationRoot(LocationConfig &location,
                         std::vector<std::string>::iterator &it,
                         unsigned int &exist_flag);
  void parseLocationAllowedMethods(LocationConfig &location,
                                   std::vector<std::string>::iterator &it);
  void parseLocationRedirect(LocationConfig &location,
                             std::vector<std::string>::iterator &it);

  // serverのparse
  void parseServer(ServerConfig &server, std::vector<std::string>::iterator &it,
                   std::vector<std::string>::iterator &ite,
                   unsigned int &exist_flag);
  void parseClientBodySizeLimit(ServerConfig &server,
                                std::vector<std::string>::iterator &it,
                                unsigned int &exist_flag);
  void parseErrorPages(ServerConfig &server,
                       std::vector<std::string>::iterator &it);
  void parseRoot(ServerConfig &server, std::vector<std::string>::iterator &it,
                 unsigned int &exist_flag);
  void parseServerName(ServerConfig &server,
                       std::vector<std::string>::iterator &it);
  void parseListen(ServerConfig &server, std::vector<std::string>::iterator &it,
                   unsigned int &exist_flag);
  void parseTokens(std::vector<std::string> tokens);

  // validation
  void serverValidate(const ServerConfig &server, const int &exist_flag);
  bool isValidStatus(const std::map<int, std::string> &config_map);
  void locationValidate(const LocationConfig &location);
  bool isValidVector(const std::vector<std::string> vec_to_check,
                     const std::vector<std::string> valid_vec);
  bool isDupLocation(const ServerConfig &server);
  bool isValidRoot(const std::string &root);
  bool isValidErrorPages(const std::map<int, std::string> &error_pages);

  // token化のためのutils
  std::string readFile(std::string const file);
  std::vector<std::string> isspaceSplit(std::string const str);

  // utils
  static int ft_stoi(std::string const &str);
  int countContents(std::vector<std::string>::iterator it);

  // 使わない関数
  ConfigParser(ConfigParser const &other);
  ConfigParser &operator=(ConfigParser const &other);
};

#endif
