#ifndef SRC_HTTPREQUEST_HPP_
#define SRC_HTTPREQUEST_HPP_

#include <map>
#include <sstream>
#include <string>

class HttpRequest {
 public:
  HttpRequest();
  ~HttpRequest();

  // アクセッサー
  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void setIsBadRequest(bool is_bad_request_);
  // デバッグ用
  std::string toString() const;

 private:
  HttpRequest(const HttpRequest& other);
  HttpRequest& operator=(const HttpRequest& other);

  std::string method_;
  std::string path_;
  std::string version_;
  std::string host_;
  int port_;
  std::string connection_;
  std::string body_;
  bool is_chanked_;
  bool is_bad_request_;
};

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs);

#endif  // SRC_HTTPREQUEST_HPP_
