#include "HttpRequest.hpp"

static std::ostream &operator<<(std::ostream &stream,
                                const HttpRequest::HeaderFieldMap &rhs) {
  HttpRequest::HeaderFieldMap::const_iterator it = rhs.begin();
  HttpRequest::HeaderFieldMap::const_iterator ite = rhs.end();
  for (; it != ite; ++it) {
    stream << it->first << ": " << it->second << std::endl;
  }
  return stream;
}

HttpRequest::HttpRequest(const ServerConfig &server_conf)
    : server_config(server_conf), response_status_code(HttpStatus::OK) {}

HttpRequest::~HttpRequest() {}

std::ostream &operator<<(std::ostream &stream, const HttpRequest &rhs) {
  stream << "###Parsed Request###" << std::endl
         << "#RequestLine" << std::endl
         << "method: " << rhs.request_line.method << std::endl
         << "uri: " << rhs.request_line.uri << std::endl
         << "version: " << rhs.request_line.version << std::endl
         << std::endl
         << "#HeaderFields" << std::endl
         << rhs.name_value_map << std::endl
         << "#Status" << std::endl
         << "status_code: " << rhs.response_status_code << std::endl
         << "##################" << std::endl
         << std::endl;
  return stream;
}
