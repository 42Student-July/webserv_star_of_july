#include "HttpRequest.hpp"

static std::ostream &operator<<(std::ostream &stream,
                                const HttpRequest::HeaderFieldMap &rhs) {
  HttpRequest::HeaderFieldMap::const_iterator it = rhs.cbegin();
  HttpRequest::HeaderFieldMap::const_iterator ite = rhs.cend();
  for (; it != ite; ++it) {
    stream << it->first << ": " << it->second << std::endl;
  }
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const HttpRequest &rhs) {
  stream << "###Parsed Request###" << std::endl
         << "#RequestLine" << std::endl
         << "method: " << rhs.method << std::endl
         << "uri: " << rhs.uri << std::endl
         << "versin: " << rhs.version << std::endl
         << std::endl
         << "#HeaderFields" << std::endl
         << rhs.name_value_map << std::endl
         << "#Status" << std::endl
         << "is_bad_request: " << std::boolalpha << rhs.is_bad_request
         << std::endl
         << "##################" << std::endl
         << std::endl;
  return stream;
}
