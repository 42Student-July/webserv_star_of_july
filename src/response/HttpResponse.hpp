#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <iostream>
#include <sstream>
#include <string>

#include "HttpResponseHeaderDTO.hpp"
class HttpResponse {
 private:
  // header attr
  std::string version_;
  std::string status_code_;
  std::string reason_phrase_;
  std::string server_;
  std::string date_;
  std::string content_type_;
  std::string content_length_;
  std::string last_modified_;
  std::string connection_;
  std::string etag_;
  std::string accept_ranges_;

  std::string header_;
  std::string body_;
  std::size_t body_size_;
  std::size_t total_size_;

 public:
  HttpResponse();
  HttpResponse(std::string header, std::string body, std::size_t body_size,
               std::size_t total_size);
  HttpResponse(HttpResponseHeaderDTO header, std::string body);
  ~HttpResponse();
  HttpResponse(const HttpResponse &other);
  HttpResponse &operator=(const HttpResponse &other);
  void printResponse();
  const std::string &Version() const;
  const std::string &StatusCode() const;
  const std::string &ReasonPhrase() const;
  const std::string &Server() const;
  const std::string &Date() const;
  const std::string &ContentType() const;
  const std::string &ContentLength() const;
  const std::string &LastModified() const;
  const std::string &Connection() const;
  const std::string &ETag() const;
  const std::string &AcceptRanges() const;
  const std::string &Body() const;
};

#endif
