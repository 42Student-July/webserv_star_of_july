#include "HttpResponseSerializer.hpp"

const std::string HttpResponseSerializer::PROTOCOL = "HTTP/";
const std::string HttpResponseSerializer::SERVER = "Server: ";
const std::string HttpResponseSerializer::DATE = "Date: ";
const std::string HttpResponseSerializer::CONTENT_TYPE = "Content-Type: ";
const std::string HttpResponseSerializer::CONTENT_LENGTH = "Content-Length: ";
const std::string HttpResponseSerializer::LAST_MODIFIED = "Last-Modified: ";
const std::string HttpResponseSerializer::CONNECTION = "Connection: ";
const std::string HttpResponseSerializer::ETAG = "ETag: ";
const std::string HttpResponseSerializer::ACCEPT_RANGES = "Accept-Ranges: ";
const std::string HttpResponseSerializer::CRLF = "\r\n";
const std::string HttpResponseSerializer::SP = " ";

HttpResponseSerializer::HttpResponseSerializer() {}
HttpResponseSerializer::~HttpResponseSerializer() {}
HttpResponseSerializer::HttpResponseSerializer(
    const HttpResponseSerializer &other) {
  *this = other;
}
HttpResponseSerializer &HttpResponseSerializer::operator=(
    const HttpResponseSerializer &other) {
  if (this != &other) {
  }
  return *this;
}

HttpResponsePlainText *HttpResponseSerializer::serialize(HttpResponse &res) {
  std::stringstream ss;

  ss << PROTOCOL << res.Version() << SP << res.StatusCode() << SP
     << res.ReasonPhrase() << CRLF << SERVER << res.Server() << CRLF << DATE
     << res.Date() << CRLF << CONTENT_TYPE << res.ContentType() << CRLF
     << CONTENT_LENGTH << res.ContentLength() << CRLF << LAST_MODIFIED
     << res.LastModified() << CRLF << CONNECTION << res.Connection() << CRLF
     << ETAG << res.ETag() << CRLF << ACCEPT_RANGES << res.AcceptRanges()
     << CRLF << CRLF << res.Body() << CRLF << CRLF;
  return new HttpResponsePlainText(ss.str(), ss.str().size());
}
