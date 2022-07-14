#ifndef SRC_HTTPREQUEST_DTO_HPP_
#define SRC_HTTPREQUEST_DTO_HPP_

#include <map>
#include <sstream>
#include <string>

typedef struct {
  std::string method;
  std::string uri;
  std::string version;
  std::string host;
  std::string connection;
  std::string body;
  // bool is_chunked; //responseにはいらない情報？あとで調べる
  bool is_bad_request;
} HttpRequestDTO;

std::ostream& operator<<(std::ostream& stream, const HttpRequestDTO& rhs);

#endif  // SRC_HTTPREQUEST_DTO_HPP_
