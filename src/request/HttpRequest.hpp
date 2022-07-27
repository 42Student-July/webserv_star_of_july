#ifndef SRC_HTTPREQUEST_HPP_
#define SRC_HTTPREQUEST_HPP_

#include <map>
#include <ostream>
#include <string>

#include "HttpStatus.hpp"
#include "RequestLine.hpp"
#include "ServerConfig.hpp"

struct HttpRequest {
  typedef std::map<std::string, std::string> HeaderFieldMap;
  typedef std::pair<std::string, std::string> HeaderFieldPair;

  HttpRequest(const ServerConfig& server_conf);
  ~HttpRequest();

  // std::string method;
  // std::string uri;
  // std::string version;
  RequestLine request_line;
  HeaderFieldMap name_value_map;
  std::string body;
  ServerConfig server_config;
  std::string response_status_code;
  size_t content_length;
  bool has_content_length;
};

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs);

#endif  // SRC_HTTPREQUEST_HPP_
