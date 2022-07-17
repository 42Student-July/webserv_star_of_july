#ifndef SRC_HTTPREQUEST_HPP_
#define SRC_HTTPREQUEST_HPP_

#include <map>
#include <ostream>
#include <string>

#include "HttpStatus.hpp"
#include "ServerConfig.hpp"

struct HttpRequest {
  typedef std::map<std::string, std::string> HeaderFieldMap;
  typedef std::pair<std::string, std::string> HeaderFieldPair;

  HttpRequest(const ServerConfig& server_conf);
  ~HttpRequest();

  std::string method;
  std::string uri;
  std::string version;
  HeaderFieldMap name_value_map;
  std::string body;
  bool is_bad_request;
  ServerConfig server_config;
  std::string status;  // responseのHttpStatus.hppにある文字列が入る
};

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs);

#endif  // SRC_HTTPREQUEST_HPP_
