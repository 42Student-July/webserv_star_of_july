#include "RequestHeader.hpp"

RequestHeader::RequestHeader() {}

RequestHeader::RequestHeader(const RequestLine& request_line,
                             const HeaderFieldMap& headers)
    : request_line_(request_line), header_map_(headers) {}

RequestHeader::~RequestHeader() {}

RequestHeader::RequestHeader(const RequestHeader& other) { *this = other; }

RequestHeader& RequestHeader::operator=(const RequestHeader& other) {
  if (this != &other) {
    request_line_ = other.request_line_;
    header_map_ = other.header_map_;
  }
  return *this;
}

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

bool RequestHeader::transferEncodingIsChunked() const {
  HeaderFieldMap::const_iterator it = header_map_.find("transfer-encoding");
  if (it != header_map_.end() && it->second == "chunked") {
    // validateしてない
    return true;
  } else {
    return false;
  }
}

static std::ostream& operator<<(std::ostream& stream,
                                const HeaderFieldMap& rhs) {
  HeaderFieldMap::const_iterator it = rhs.begin();
  HeaderFieldMap::const_iterator ite = rhs.end();
  stream << "#HeaderFields" << std::endl;
  for (; it != ite; ++it) {
    stream << it->first << ": " << it->second << std::endl;
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const RequestLine& rhs) {
  stream << "#RequestLine" << std::endl
         << "method: " << rhs.method << std::endl
         << "uri: " << rhs.uri << std::endl
         << "version: " << rhs.version << std::endl;
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const RequestHeader& rhs) {
  stream << rhs.requestLine() << std::endl << rhs.headerMap() << std::endl;
  return stream;
}
