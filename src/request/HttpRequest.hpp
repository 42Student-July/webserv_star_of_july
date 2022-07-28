#ifndef SRC_HTTPREQUEST_HPP_
#define SRC_HTTPREQUEST_HPP_

#include <map>
#include <ostream>
#include <string>

#include "HttpStatus.hpp"
#include "ServerConfig.hpp"

typedef std::map<std::string, std::string> HeaderFieldMap;
typedef std::pair<std::string, std::string> HeaderFieldPair;

struct RequestLine {
  std::string method;
  std::string uri;
  std::string version;
};

struct RequestHeader {
  RequestLine request_line;
  HeaderFieldMap name_value_map;
};

struct HttpRequest {
  HttpRequest(const ServerConfig& server_conf);
  ~HttpRequest();

  RequestHeader header;
  std::string body;
  ServerConfig server_config;
  std::string response_status_code;
  size_t content_length;
  bool has_content_length;
};

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs);

#endif  // SRC_HTTPREQUEST_HPP_
