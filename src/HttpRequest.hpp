#ifndef HTTPSERVER_SRC_HTTPREQUEST_HPP_
#define HTTPSERVER_SRC_HTTPREQUEST_HPP_

#include <map>
#include <sstream>
#include <string>

class HttpRequest {
  typedef std::map<std::string, std::string> HeaderFieldMap;

 public:
  HttpRequest();
  ~HttpRequest();

  // アクセッサー
  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void setHeaderFieldMap(const HeaderFieldMap& field_to_value);
  void setIsValid(bool is_valid);
  // デバッグ用
  std::string toString() const;

 private:
  HttpRequest(const HttpRequest& other);
  HttpRequest& operator=(const HttpRequest& other);

  std::string method_;
  std::string uri_;
  std::string version_;
  HeaderFieldMap field_to_value_;
  std::string body_;
  bool is_valid_;
};

std::ostream& operator<<(std::ostream& stream, const HttpRequest& rhs);

#endif  // HTTPSERVER_SRC_HTTPREQUEST_HPP_
