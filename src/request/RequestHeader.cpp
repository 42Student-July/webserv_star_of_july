#include "RequestHeader.hpp"

RequestHeader::RequestHeader() {}

RequestHeader::RequestHeader(const RequestLine& request_line,
                             const HeaderFieldMap& headers)
    : request_line_(request_line), header_map_(headers) {}

RequestHeader::~RequestHeader() {}

const RequestLine& RequestHeader::requestLine() const { return request_line_; }

const HeaderFieldMap& RequestHeader::headerMap() const { return header_map_; }

size_t RequestHeader::contentLength() const {
  HeaderFieldMap::const_iterator it = header_map_.find("content-length");
  if (it != header_map_.end()) {
    // validateしてない
    return utility::stoi(it->second);
  } else {
    return 0;
  }
}

static std::ostream& operator<<(std::ostream& stream,
                                const HeaderFieldMap& rhs) {
  HeaderFieldMap::const_iterator it = rhs.begin();
  HeaderFieldMap::const_iterator ite = rhs.end();
  for (; it != ite; ++it) {
    stream << it->first << ": " << it->second << std::endl;
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const RequestHeader& rhs) {
  stream << "#RequestLine" << std::endl
         << "method: " << rhs.requestLine().method << std::endl
         << "uri: " << rhs.requestLine().uri << std::endl
         << "version: " << rhs.requestLine().version << std::endl
         << std::endl
         << "#HeaderFields" << std::endl
         << rhs.headerMap() << std::endl;
  return stream;
}
