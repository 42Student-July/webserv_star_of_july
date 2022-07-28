#include "HttpRequestParser.hpp"
#include "utility.hpp"

HttpRequestParser::HttpRequestParser() : parse_status_(PARSE_HEADER) {}

HttpRequestParser::~HttpRequestParser() {}

#include <cstdio>

// 2つの引数はコンストラクタで渡した方が読みやすいかも。
HttpRequest *HttpRequestParser::parse(const std::string unparsed_str,
                                      const ServerConfig &server_config) {
  HttpRequest *req = new HttpRequest(server_config);
  RequestHeader header;

  try {
    if (parse_status_ == PARSE_HEADER) {
      req->header = parseRequestHeader(unparsed_str);
    }
    if (parse_status_ == PARSE_BODY) {
      req->body = parseBody(req->header.contentLength());
    }
    if (parse_status_ == PARSE_CHUNKED_BODY) {
      unparsed_body_ += unparsed_str;
      if (unparsed_body_.size() > kMaxBodyLength) {
        throw ParseErrorExeption(HttpStatus::PAYLOAD_TOO_LARGE,
                                 "body is too long");
      }
      if (unparsed_body_.find("0\r\n\r\n") == std::string::npos) {
        req->body = parseChunkedBody(unparsed_body_);
      }
    }
  } catch (const ParseErrorExeption &e) {
    changeStatus(PARSE_ERROR);
    req->response_status_code = e.getErrorStatus();
    std::cerr << e.what() << std::endl;
  } catch (const std::exception &e) {
    changeStatus(PARSE_ERROR);
    req->response_status_code = HttpStatus::INTERNAL_SERVER_ERROR;
    std::cerr << e.what() << std::endl;
  }
  return req;
}

RequestHeader HttpRequestParser::parseRequestHeader(
    const std::string &unparsed_str) {
  RequestHeaderParser rh_parser;
  size_t header_end = unparsed_str.find(CRLF + CRLF);
  if (header_end == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No header_end");
  }
  std::string unparsed_header = unparsed_str.substr(0, header_end + 4);
  RequestHeader header = rh_parser.parse(unparsed_header);
  if (header.transferEncodingIsChunked()) {
    changeStatus(PARSE_CHUNKED_BODY);
  } else {
    changeStatus(PARSE_BODY);
  }
  unparsed_body_ = unparsed_str.substr(header_end + 4);
  return header;
}

std::string HttpRequestParser::parseBody(size_t content_length) {
  MessageBodyParser mb_parser;
  std::string body = mb_parser.parseBody(unparsed_body_, content_length);
  changeStatus(PARSE_DONE);
  return body;
}

std::string HttpRequestParser::parseChunkedBody(
    const std::string &unparsed_str) {
  MessageBodyParser mb_parser;
  std::string body = mb_parser.parseChunkedBody(unparsed_str);
  changeStatus(PARSE_DONE);
  return body;
}

void HttpRequestParser::changeStatus(Status next_status) {
  parse_status_ = next_status;
}
