#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : is_bad_request_(false) {}

HttpRequest::~HttpRequest() {}

void HttpRequest::setMethod(const std::string &method) { method_ = method; }

void HttpRequest::setUri(const std::string &path) { path_ = path; }

void HttpRequest::setVersion(const std::string &version) { version_ = version; }

void HttpRequest::setIsBadRequest(bool is_bad_request) {
  is_bad_request_ = is_bad_request;
}

std::string HttpRequest::toString() const {
  std::ostringstream oss;

  oss << "###Request Info###" << std::endl
      << "method: " << method_ << std::endl
      << "uri: " << path_ << std::endl
      << "versin: " << version_ << std::endl
      << "state: " << std::boolalpha << is_bad_request_ << std::endl
      << "##################" << std::endl
      << std::endl;
  return oss.str();
}

std::ostream &operator<<(std::ostream &stream, const HttpRequest &rhs) {
  stream << rhs.toString();

  return stream;
}
