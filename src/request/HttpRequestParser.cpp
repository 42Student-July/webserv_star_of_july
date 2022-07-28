#include "HttpRequestParser.hpp"
#include "utility.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

#include <cstdio>

// 2つの引数はコンストラクタで渡した方が読みやすいかも。
HttpRequest *HttpRequestParser::parse(const std::string unparsed_str,
                                      const ServerConfig &server_config) {
  HttpRequest *req = new HttpRequest(server_config);
  RequestHeader header;

  try {
    validateRequestLength(unparsed_str);
    req->header = parseRequestHeader(unparsed_str);
    req->body = parseMessageBody(unparsed_str, req->header);
  } catch (const ParseErrorExeption &e) {
    req->response_status_code = e.getErrorStatus();
    std::cerr << e.what() << std::endl;
  } catch (const std::exception &e) {
    req->response_status_code = HttpStatus::INTERNAL_SERVER_ERROR;
    std::cerr << e.what() << std::endl;
  }
  return req;
}

// http://nginx.org/en/docs/http/ngx_http_core_module.html#client_header_buffer_size
void HttpRequestParser::validateRequestLength(const std::string &unparsed_str) {
  StringPos header_end = unparsed_str.find(CRLF + CRLF);
  if (header_end == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No header_end");
  }
  if (header_end > kMaxHeaderLength) {
    throw ParseErrorExeption(HttpStatus::REQUEST_HEADER_FIELD_TOO_LARGE,
                             "request is too long");
  }
  size_t body_len = unparsed_str.size() - header_end;
  if (body_len > kMaxBodyLength) {
    throw ParseErrorExeption(HttpStatus::PAYLOAD_TOO_LARGE, "body is too long");
  }
}

RequestHeader HttpRequestParser::parseRequestHeader(
    const std::string &unparsed_str) {
  RequestHeaderParser rh_parser;
  size_t header_len = unparsed_str.find(CRLF + CRLF) + 4;
  std::string unparsed_header = unparsed_str.substr(0, header_len);

  return rh_parser.parse(unparsed_header);
}

std::string HttpRequestParser::parseMessageBody(const std::string &unparsed_str,
                                                const RequestHeader &header) {
  StringPos body_begin = unparsed_str.find(CRLF + CRLF) + 4;
  std::string unparsed_body = unparsed_str.substr(body_begin);
  if (unparsed_body.empty()) {
    return "";
  }
  MessageBodyParser mb_parser;
  return mb_parser.parse(unparsed_body, false, header.contentLength());
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
// 現在のオフセットから一行読み取る関数。読み取ったら次の行頭にoffsetを進める
std::string HttpRequestParser::getLine(const std::string &unparsed_str,
                                       StringPos *offset) {
  StringPos crlf_pos = unparsed_str.find(CRLF, *offset);
  if (crlf_pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "getLine() error");
  }
  std::string line = unparsed_str.substr(*offset, crlf_pos - *offset);
  *offset = crlf_pos + 2;
  return line;
}
