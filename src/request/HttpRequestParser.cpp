#include "HttpRequestParser.hpp"
#include "utility.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

#include <cstdio>

// 2つの引数はコンストラクタで渡した方が読みやすいかも。
HttpRequest *HttpRequestParser::parse(const std::string buffer,
                                      const ServerConfig &server_config) {
  HttpRequest *req = new HttpRequest(server_config);
  RequestHeader header;

  try {
    validateRequestLength(buffer);
    req->header = parseRequestHeader(buffer);
    StringPos body_begin = buffer.find(CRLF + CRLF) + 4;
    body_buffer_ = buffer.substr(body_begin);
    setContentLengthInfo(req->header.name_value_map, req);
    req->body = parseBody(body_buffer_);
  } catch (const ParseErrorExeption &e) {
    req->response_status_code = e.getErrorStatus();
    std::cerr << e.what() << std::endl;
  } catch (
      const std::exception &
          e) {  // (...)ですべての例外をキャッチした方がいいかも。でもe.what()でエラーメッセージを見たいのでこちらで対応
    req->response_status_code = HttpStatus::INTERNAL_SERVER_ERROR;
    std::cerr << e.what() << std::endl;
  }
  return req;
}

const std::string &HttpRequestParser::getBodyBuffer() const {
  return body_buffer_;
}

// http://nginx.org/en/docs/http/ngx_http_core_module.html#client_header_buffer_size
void HttpRequestParser::validateRequestLength(const std::string &buffer) {
  StringPos header_end = buffer.find(CRLF + CRLF);
  if (header_end == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No header_end");
  }
  if (header_end > kMaxHeaderLength) {
    throw ParseErrorExeption(HttpStatus::REQUEST_HEADER_FIELD_TOO_LARGE,
                             "request is too long");
  }
  size_t body_len = buffer.size() - header_end;
  if (body_len > kMaxBodyLength) {
    throw ParseErrorExeption(HttpStatus::PAYLOAD_TOO_LARGE, "body is too long");
  }
}

RequestHeader HttpRequestParser::parseRequestHeader(const std::string &buffer) {
  RequestHeaderParser rh_parser;
  size_t header_len = buffer.find(CRLF + CRLF) + 4;
  std::string unparsed_header = buffer.substr(0, header_len);

  return rh_parser.parse(unparsed_header);
}

std::string HttpRequestParser::parseBody(const std::string &buffer) {
  // bodyがないケース
  if (buffer.empty()) {
    return "";
  }

  StringPos body_end = buffer.find(CRLF);
  if (body_end == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No body_end");
  }
  return buffer.substr(0, body_end);
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
// 現在のオフセットから一行読み取る関数。読み取ったら次の行頭にoffsetを進める
std::string HttpRequestParser::getLine(const std::string &buffer,
                                       StringPos *offset) {
  StringPos crlf_pos = buffer.find(CRLF, *offset);
  if (crlf_pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "getLine() error");
  }
  std::string line = buffer.substr(*offset, crlf_pos - *offset);
  *offset = crlf_pos + 2;
  return line;
}

#include <cstdio>
// 動かすために.あとでどこのメソッドにするかふくめ、リファクタする
void HttpRequestParser::setContentLengthInfo(HeaderFieldMap &headerfield_map,
                                             HttpRequest *req) {
  if (headerfield_map.find("content-length") != headerfield_map.end()) {
    // validateしてない
    req->content_length = utility::stoi(headerfield_map["content-length"]);
    req->has_content_length = true;
  } else {
    req->content_length = body_buffer_.size();
    req->has_content_length = false;
  }
}
