#ifndef HTTPSERVER_SRC_HTTPRESPONSE_HPP_
#define HTTPSERVER_SRC_HTTPRESPONSE_HPP_

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

class HttpResponse {
  // HTTPの教科書に載ってたもの。提出するときは対応したものだけにする？
  enum HttpStatus {
    HTTP_OK = 200,
    HTTP_NO_CONTENT = 204,
    HTTP_PARTIAL_CONTENT = 206,
    HTTP_MOVED_PERMANENTLY = 301,
    HTTP_FOUND = 302,
    HTTP_SEE_OTHER = 303,
    HTTP_NOT_MODIFIED = 304,
    HTTP_TEMPORARY_REDIRECT = 307,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_SERVICE_UNAVAILABLE = 503,
  };

 public:
  // デフォルトはHTTP/1.1 200 OK
  HttpResponse();
  // HttpResponse(const std::string&  version, const std::string &);
  ~HttpResponse();

  // void setVersion();
  // void setStatusCode();
  // void appendHeaderField();
  void setBody(const std::string& body);
  std::string toString() const;

 private:
  // とりあえずコピーしない想定
  HttpResponse(const HttpResponse& other);
  HttpResponse& operator=(const HttpResponse& other);

  static const char* getReasonForStatus(HttpStatus status);

  std::string version_;
  HttpStatus status_code_;
  std::string reason_phrase_;
  std::string content_length_;
  std::string body_;

  static const char* kHttpReasonOk;
  static const char* kHttpReasonNoContent;
  static const char* kHttpReasonPartialContent;
  static const char* kHttpReasonMovedPermanently;
  static const char* kHttpReasonFound;
  static const char* kHttpReasonSeeOther;
  static const char* kHttpReasonNotModified;
  static const char* kHttpReasonTemporaryRedirect;
  static const char* kHttpReasonBadRequest;
  static const char* kHttpReasonUnauthorized;
  static const char* kHttpReasonForbidden;
  static const char* kHttpReasonNotFound;
  static const char* kHttpReasonInternalServerError;
  static const char* kHttpReasonServiceUnavailable;
  static const char* kHttpReasonUnknown;
};

#endif  // HTTPSERVER_SRC_HTTPRESPONSE_HPP_
