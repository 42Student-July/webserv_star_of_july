#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : is_valid_(false) {}

HttpRequest::~HttpRequest() {}

void HttpRequest::setMethod(const std::string& method) { method_ = method; }

void HttpRequest::setUri(const std::string& uri) { uri_ = uri; }

void HttpRequest::setVersion(const std::string& version) { version_ = version; }

void HttpRequest::setHeaderFieldMap(const HeaderFieldMap& field_to_value) {
  field_to_value_ = field_to_value;
}
void HttpRequest::setIsValid(bool is_valid) { is_valid_ = is_valid; }

std::string HttpRequest::toString() const {
  std::ostringstream oss;

  oss << "###Request Info###" << std::endl
      << "method: " << method_ << std::endl
      << "uri: " << uri_ << std::endl
      << "versin: " << version_ << std::endl
      << "state: " << std::boolalpha << is_valid_ << std::endl
      << "##################" << std::endl;
  return oss.str();
}

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs) {
  stream << rhs.toString();

  return stream;
}
