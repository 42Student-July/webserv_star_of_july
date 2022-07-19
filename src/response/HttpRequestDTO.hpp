#ifndef HTTPREQUESTDTO_HPP
#define HTTPREQUESTDTO_HPP

#include <string>

struct HttpRequestDTO {
  std::string method;
  std::string path;
  std::string version;
  std::string connection;
  std::string body;

  std::string authorization;
  std::string content_length;
  std::string content_type;
  std::string accept;
  std::string forwarded;
  std::string referer;
  std::string user_agent;
  std::string x_forwarded_for;
  // TODO: 後で消す
  std::string file;

  // Serverの情報
  std::string host;
  size_t port;
  std::vector<std::string> servernames;

  // Parse時点でのHttpStatus
  std::string response_status_code;
};

#endif
