#include "RequestLineParser.hpp"

RequestLineParser::RequestLineParser() {}

RequestLineParser::~RequestLineParser() {}

RequestLine RequestLineParser::parse(const std::string& line) {
  RequestLine request_line;
  StringPos offset = 0;

  request_line.method = parseMethod(line, &offset);
  request_line.uri = parseUri(line, &offset);
  request_line.version = parseHttpVersion(line, offset);
  validateRequestLine(request_line);
  return request_line;
}

std::string RequestLineParser::parseMethod(const std::string& line,
                                           StringPos* offset) {
  StringPos method_end = line.find_first_of(" ");

  *offset = method_end;
  return line.substr(0, method_end);
}
std::string RequestLineParser::parseUri(const std::string& line,
                                        StringPos* offset) {
  StringPos uri_begin = line.find_first_not_of(" ", *offset);
  StringPos uri_end = line.find_first_of(" ", uri_begin);

  *offset = uri_end;
  return line.substr(uri_begin, uri_end - uri_begin);
}

std::string RequestLineParser::parseHttpVersion(const std::string& line,
                                                StringPos offset) {
  return line.substr(offset + 1);
}

void RequestLineParser::validateRequestLine(const RequestLine& request_line) {
  validateMethod(request_line.method);
  validateUri(request_line.uri);
  validateHttpVersion(request_line.version);
}

void RequestLineParser::validateMethod(const std::string& method) {
  if (method.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No Method");
  }
}

void RequestLineParser::validateUri(const std::string& uri) {
  if (uri.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No Uri");
  }
}

void RequestLineParser::validateHttpVersion(const std::string& http_version) {
  if (http_version.size() != 8) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "HttpVersion length error");
  }
  if (http_version.compare(0, 5, "HTTP/") != 0) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "Invalid Protocol");
  }

  std::string version_num = http_version.substr(5);
  if (version_num[1] != '.') {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, ". is not found");
  }
  if (!isdigit(version_num[0]) || !isdigit(version_num[2])) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "version has other than digit");
  }
  if (version_num[0] != '1' || version_num[2] != '1') {
    throw ParseErrorExeption(HttpStatus::HTTP_VERSION_NOT_SUPPORTED,
                             "version is not supported");
  }
}
