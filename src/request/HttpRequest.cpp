#include "HttpRequest.hpp"

HttpRequest::HttpRequest(const ServerConfig &server_conf)
    : server_config(server_conf), response_status_code(HttpStatus::OK) {}

HttpRequest::~HttpRequest() {}

std::ostream &operator<<(std::ostream &stream, const HttpRequest &rhs) {
  stream << "###Parsed Request###" << std::endl
         << rhs.header << std::endl
         << "#Body" << std::endl
         << rhs.body << std::endl
         << "#Status" << std::endl
         << "status_code: " << rhs.response_status_code << std::endl
         << "##################" << std::endl
         << std::endl;
  return stream;
}
