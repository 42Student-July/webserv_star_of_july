#ifndef SRC_REQUESTHEADER_HPP_
#define SRC_REQUESTHEADER_HPP_

#include <map>
#include <ostream>
#include <string>

#include "HttpStatus.hpp"
#include "utility.hpp"

typedef std::map<std::string, std::string> HeaderFieldMap;
typedef std::pair<std::string, std::string> HeaderFieldPair;

struct RequestLine {
  std::string method;
  std::string uri;
  std::string version;
};

class RequestHeader {
 public:
  RequestHeader();
  RequestHeader(const RequestLine& request_line, const HeaderFieldMap& headers,
                const std::string& host);
  ~RequestHeader();
  RequestHeader(const RequestHeader& other);
  RequestHeader& operator=(const RequestHeader& other);
  const RequestLine& requestLine() const;
  const HeaderFieldMap& headerMap() const;
  const std::string& host() const;
  size_t contentLength() const;
  bool transferEncodingIsChunked() const;

 private:
  RequestLine request_line_;
  HeaderFieldMap header_map_;
  std::string host_;
};

std::ostream& operator<<(std::ostream& stream, const RequestLine& rhs);
std::ostream& operator<<(std::ostream& stream, const RequestHeader& rhs);

#endif  // SRC_REQUESTHEADER_HPP_
