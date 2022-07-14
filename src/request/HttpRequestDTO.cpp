#include "HttpRequestDTO.hpp"

static std::ostream &operator<<(std::ostream &stream,
                                const std::vector<std::string> &rhs) {
  std::vector<std::string>::const_iterator it = rhs.cbegin();
  std::vector<std::string>::const_iterator ite = rhs.cend();
  for (size_t cnt = 1; it != ite; ++it, ++cnt) {
    stream << "servername" << cnt << ": " << *it << std::endl;
  }
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const HttpRequestDTO &rhs) {
  stream << "###RequestDTO Info###" << std::endl
         << "##Request info" << std::endl
         << "#RequestLine" << std::endl
         << "method: " << rhs.method << std::endl
         << "path: " << rhs.path << std::endl
         << "version: " << rhs.version << std::endl
         << std::endl
         << "#HeaderFields" << std::endl
         << "authorization: " << rhs.authorization << std::endl
         << "content_length: " << rhs.content_length << std::endl
         << "content_type: " << rhs.content_type << std::endl
         << "accept: " << rhs.accept << std::endl
         << "forwarded: " << rhs.forwarded << std::endl
         << "referer: " << rhs.referer << std::endl
         << "user_agent: " << rhs.user_agent << std::endl
         << "x_forwarded_for: " << rhs.x_forwarded_for << std::endl
         << "#Body" << std::endl
         << rhs.body << std::endl
         << "#Status" << std::endl
         << "is_bad_request: " << std::boolalpha << rhs.is_bad_request
         << std::endl
         << "##Server Info" << std::endl
         << "host: " << rhs.host << std::endl
         << "port: " << rhs.port << std::endl
         << rhs.servernames << std::endl
         << "##################" << std::endl
         << std::endl;
  return stream;
}
