#include "HttpRequestParser.hpp"
#include "utility.hpp"

HttpRequestParser::HttpRequestParser() : raw_buffer_(""), offset_(0) {}

HttpRequestParser::~HttpRequestParser() {}

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
    std::cerr << e.what() << std::endl;
  }
  return request;
}

void HttpRequestParser::parseRequestLine(HttpRequest* request) {
  RequestLineParser rl_parser;

  request->request_line = rl_parser.parse(getLine());
}

void HttpRequestParser::parseHeaderField(HttpRequest* request) {
  for (std::string line = getLine(); line.size() != 0; line = getLine()) {
    HeaderFieldPair name_value_pair = makeHeaderFieldPair(line);

    validateHeaderField(name_value_pair);
    request->name_value_map.insert(name_value_pair);
  }
  validateHeaderFields(request->name_value_map);
}

void HttpRequestParser::validateHeaderFields(const HeaderFieldMap& headers) {
  HeaderFieldMap::const_iterator it = headers.find("Host");
  if (it == headers.end()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "no host");
  }
}

bool isWS(int c) { return (c == ' ' || c == '\t'); }

bool HttpRequestParser::isHeaderDelimiter(int c) {
  return Delimiters.find(c) != std::string::npos;
}

bool HttpRequestParser::isHeaderTokenChar(int c) {
  if (isHeaderDelimiter(c) || c == ' ') {
    return false;
  }
  return isprint(c);
}

bool HttpRequestParser::isHeaderToken(const std::string& str) {
  if (str.empty()) {
    return false;
  }
  for (StringPos pos = 0; pos <= str.size() - 1; ++pos) {
    if (!isHeaderTokenChar(str[pos])) {
      return false;
    }
  }
  return true;
}

void HttpRequestParser::validateHeaderField(
    const HeaderFieldPair& headerfield_pair) {
  std::string field_name = headerfield_pair.first;
  std::string field_value = headerfield_pair.second;

  if (field_name.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "header has no field name");
  }
  if (isWS(field_name[field_name.size() - 1])) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "header has space before colon");
  }
  if (!isHeaderToken(field_name)) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "field_name is not header token");
  }
  // if (headerfield_map.find(field_name) != headerfield_map.end()) {
  //   throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
  //                            "field_name has already existed");
  // }
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
HttpRequestParser::HeaderFieldPair HttpRequestParser::makeHeaderFieldPair(
    const std::string& line) {
  StringPos name_end = line.find_first_of(":");
  if (name_end == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "header has no colon");
  }
  std::string name = line.substr(0, name_end);
  std::string value = utility::trimCopyIf(line.substr(name_end + 1), WS);

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
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "getLine() error");
  }
  std::string line = raw_buffer_.substr(offset_, crlf_pos - offset_);
  offset_ = crlf_pos + 2;
  return line;
}
