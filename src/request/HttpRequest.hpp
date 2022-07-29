#ifndef SRC_HTTPREQUEST_HPP_
#define SRC_HTTPREQUEST_HPP_

#include <map>
#include <ostream>
#include <string>

#include "HttpStatus.hpp"
#include "RequestHeader.hpp"
#include "ServerConfig.hpp"

struct HttpRequest {
  HttpRequest(const ServerConfig& server_conf);
  ~HttpRequest();

  RequestHeader header;
  std::string body;
  ServerConfig server_config;
  std::string response_status_code;
};

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs);

#endif  // SRC_HTTPREQUEST_HPP_
