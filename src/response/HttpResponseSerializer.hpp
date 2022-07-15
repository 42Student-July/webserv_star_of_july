#ifndef HTTPRESPONSESERIALIZER_HPP
#define HTTPRESPONSESERIALIZER_HPP

#include <sstream>

#include "HttpResponse.hpp"
#include "HttpResponsePlainText.hpp"

class HttpResponseSerializer {
 private:
  static const std::string PROTOCOL;
  static const std::string SERVER;
  static const std::string DATE;
  static const std::string CONTENT_TYPE;
  static const std::string CONTENT_LENGTH;
  static const std::string LAST_MODIFIED;
  static const std::string CONNECTION;
  static const std::string ETAG;
  static const std::string ACCEPT_RANGES;
  static const std::string CRLF;
  static const std::string SP;

 public:
  HttpResponseSerializer();
  ~HttpResponseSerializer();
  HttpResponseSerializer(const HttpResponseSerializer &other);
  HttpResponseSerializer &operator=(const HttpResponseSerializer &other);
  HttpResponsePlainText *serialize(HttpResponse &res);
};

#endif
