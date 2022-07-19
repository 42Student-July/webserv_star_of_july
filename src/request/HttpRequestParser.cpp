#include "HttpRequestParser.hpp"

const std::string HttpRequestParser::CRLF = "\r\n";
const std::string HttpRequestParser::WS = " \t";

HttpRequestParser::HttpRequestParser() : raw_buffer_(""), offset_(0) {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequestParser::ParseErrorExeption::ParseErrorExeption(
    const std::string& error_status, const std::string& reason)
    : std::runtime_error(reason), error_status_(error_status) {}

HttpRequestParser::ParseErrorExeption::~ParseErrorExeption() throw() {}

const std::string& HttpRequestParser::ParseErrorExeption::getErrorStatus()
    const {
  return error_status_;
}

// 2つの引数はコンストラクタで渡した方が読みやすいかも。
HttpRequest* HttpRequestParser::parse(const char* request_str,
                                      const ServerConfig& server_config) {
  raw_buffer_ = request_str;
  HttpRequest* request = new HttpRequest(server_config);

  try {
    parseRequestLine(request);
    parseHeaderField(request);
    parseBody(request);
  } catch (const ParseErrorExeption& e) {
    request->response_status_code = e.getErrorStatus();
    // std::cerr << e.what() << std::endl;
  }
  return request;
}

void HttpRequestParser::parseRequestLine(HttpRequest* request) {
  StringPos offset = 0;
  std::string line = getLine();

  offset = parseMethod(line, request);
  offset = parseUri(line, request, offset);
  offset = parseHttpVersion(line, request, offset);
  validateRequestLine(request);
}

HttpRequestParser::StringPos HttpRequestParser::parseMethod(
    const std::string& request_line, HttpRequest* request) {
  StringPos method_end = request_line.find_first_of(" ");

  request->method = request_line.substr(0, method_end);
  return method_end;
}

HttpRequestParser::StringPos HttpRequestParser::parseUri(
    const std::string& request_line, HttpRequest* request, StringPos offset) {
  StringPos uri_begin = request_line.find_first_not_of(" ", offset);
  // StringPos uri_end = request_line.find_last_of(" ");
  StringPos uri_end = request_line.find_first_of(" ", uri_begin);

  request->uri = request_line.substr(uri_begin, uri_end - uri_begin);
  return uri_end;
}

HttpRequestParser::StringPos HttpRequestParser::parseHttpVersion(
    const std::string& request_line, HttpRequest* request, StringPos offset) {
  request->version = request_line.substr(offset + 1);
  return std::string::npos;
}

void HttpRequestParser::validateRequestLine(HttpRequest* request) {
  if (request->method.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No Method");
  }
  if (request->uri.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No Uri");
  }
  if (request->version.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No Version");
  }
  // httpversion
  if (request->version.size() != 8) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "HttpVersion length error");
  }
  if (request->version.compare(0, 4, "HTTP") != 0) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "Invalid Protocol");
  }
  if (request->version.compare(4, 1, "/") != 0) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "/ is not found");
  }

  std::string version_num = request->version.substr(5);
  std::cerr << version_num << std::endl;
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

void HttpRequestParser::parseHeaderField(HttpRequest* request) {
  for (std::string line = getLine(); line.size() != 0; line = getLine()) {
    HeaderFieldPair name_value_pair = makeHeaderFieldPair(line);

    request->name_value_map.insert(name_value_pair);
  }
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
HttpRequestParser::HeaderFieldPair HttpRequestParser::makeHeaderFieldPair(
    const std::string& line) {
  StringPos name_end = line.find_first_of(":");
  StringPos value_begin = line.find_first_not_of(WS, name_end + 1);
  std::string name = line.substr(0, name_end);
  std::string value = line.substr(value_begin);

  return HeaderFieldPair(name, value);
}

void HttpRequestParser::parseBody(HttpRequest* request) {
  // bodyがないケース
  if (offset_ == raw_buffer_.size()) {
    return;
  }
  for (std::string line = getLine(); line.size() != 0; line = getLine()) {
    request->body += line;
    request->body += "\n";
  }
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
// 現在のオフセットから一行読み取る関数。読み取ったら次の行頭にoffsetを進める
std::string HttpRequestParser::getLine() {
  StringPos crlf_pos = raw_buffer_.find(CRLF, offset_);
  if (crlf_pos == std::string::npos) {
    throw ParseErrorExeption("400", "getLine() error");
  }
  std::string line = raw_buffer_.substr(offset_, crlf_pos - offset_);
  offset_ = crlf_pos + 2;
  return line;
}
