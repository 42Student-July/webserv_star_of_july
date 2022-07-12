#include "HttpRequestDTO.hpp"

std::ostream &operator<<(std::ostream &stream, const HttpRequestDTO &rhs) {
  stream << "###Request Info###" << std::endl
         << "method: " << rhs.method << std::endl
         << "uri: " << rhs.uri << std::endl
         << "versin: " << rhs.version << std::endl
         << "is_bad_request: " << std::boolalpha << rhs.is_bad_request
         << std::endl
         << "##################" << std::endl
         << std::endl;
  return stream;
}
