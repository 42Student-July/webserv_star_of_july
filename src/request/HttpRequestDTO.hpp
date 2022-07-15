#ifndef SRC_HTTPREQUEST_DTO_HPP_
#define SRC_HTTPREQUEST_DTO_HPP_

#include <ostream>
#include <string>
#include <vector>

struct HttpRequestDTO {
  // HttpRequestの情報
  std::string method;
  std::string path;
  std::string version;
  std::string authorization;
  std::string content_length;
  std::string content_type;
  std::string accept;
  std::string forwarded;
  std::string referer;
  std::string user_agent;
  std::string x_forwarded_for;
  std::string body;
  bool is_bad_request;  // Parser時点でのbadrequestの判定
  // Serverの情報
  std::string host;
  size_t port;
  std::vector<std::string> servernames;
};

std::ostream& operator<<(std::ostream& stream, const HttpRequestDTO& rhs);

#endif  // SRC_HTTPREQUEST_DTO_HPP_
