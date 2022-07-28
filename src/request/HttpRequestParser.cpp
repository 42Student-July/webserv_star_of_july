#include "HttpRequestParser.hpp"
#include "utility.hpp"

HttpRequestParser::HttpRequestParser() : parse_status_(PARSE_HEADER) {}

HttpRequestParser::~HttpRequestParser() {}

void HttpRequestParser::parse(const std::string unparsed_str,
                              const ServerConfig &server_config) {
  // bodysizelimitまだ
  (void)server_config;
  try {
    if (parse_status_ == PARSE_DONE || parse_status_ == PARSE_ERROR) {
      clear();
    }
    if (parse_status_ == PARSE_HEADER) {
      parsed_header_ = parseRequestHeader(unparsed_str);
    }
    if (parse_status_ == PARSE_BODY) {
      parsed_body_ = parseBody(unparsed_str, parsed_header_.contentLength());
    }
    if (parse_status_ == PARSE_CHUNKED_BODY) {
      chunked_request_ += unparsed_str;
      if (chunked_request_.size() > kMaxHeaderLength + kMaxBodyLength) {
        throw ParseErrorExeption(HttpStatus::PAYLOAD_TOO_LARGE,
                                 "body is too long");
      }
      if (chunked_request_.find("0\r\n\r\n", findHeaderEnd(chunked_request_)) !=
          std::string::npos) {
        std::cerr << chunked_request_ << std::endl;
        parsed_body_ = parseChunkedBody(fetchUnparsedBody(chunked_request_));
      }
    }
  } catch (const ParseErrorExeption &e) {
    changeStatus(PARSE_ERROR);
    error_code_ = e.getErrorStatus();
    std::cerr << "Catch exeption in HttpRequestParser: " << std::endl
              << e.what() << std::endl;
  } catch (const std::exception &e) {
    changeStatus(PARSE_ERROR);
    error_code_ = HttpStatus::INTERNAL_SERVER_ERROR;
    std::cerr << "Catch exeption in HttpRequestParser: " << std::endl
              << e.what() << std::endl;
  }
}

// コンストラクタの引数増やしたい
// server_configの選択のロジックはまだ
HttpRequest *HttpRequestParser::buildRequest(
    const ServerConfig &server_config) {
  HttpRequest *req = new HttpRequest(server_config);

  req->header = parsed_header_;
  req->body = parsed_body_;
  req->response_status_code = (errorOccured()) ? error_code_ : HttpStatus::OK;
  return req;
}

bool HttpRequestParser::errorOccured() const {
  return parse_status_ == PARSE_ERROR;
}

bool HttpRequestParser::finished() const { return parse_status_ == PARSE_DONE; }

RequestHeader HttpRequestParser::parseRequestHeader(
    const std::string &unparsed_str) {
  RequestHeaderParser rh_parser;
  std::string unparsed_header = fetchUnparsedHeader(unparsed_str);
  RequestHeader header = rh_parser.parse(unparsed_header);
  if (header.transferEncodingIsChunked()) {
    changeStatus(PARSE_CHUNKED_BODY);
  } else {
    changeStatus(PARSE_BODY);
  }
  return header;
}

std::string HttpRequestParser::parseBody(const std::string &unparsed_str,
                                         size_t content_length) {
  MessageBodyParser mb_parser;
  std::string unparsed_body = fetchUnparsedBody(unparsed_str);
  std::string body = mb_parser.parseBody(unparsed_body, content_length);
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

void HttpRequestParser::clear() {
  RequestHeader header;

  parsed_header_ = header;
  parsed_body_ = "";
  chunked_request_ = "";
  error_code_ = "";
  changeStatus(PARSE_HEADER);
}

// 命名適当
HttpParser::StringPos HttpRequestParser::findHeaderEnd(
    const std::string &unparsed_req) {
  size_t pos = unparsed_req.find(CRLF + CRLF);
  if (pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No header_end");
  }
  return pos + 4;
}

std::string HttpRequestParser::fetchUnparsedHeader(
    const std::string &unparsed_req) {
  return unparsed_req.substr(0, findHeaderEnd(unparsed_req));
}

std::string HttpRequestParser::fetchUnparsedBody(
    const std::string &unparsed_req) {
  return unparsed_req.substr(findHeaderEnd(unparsed_req));
}
