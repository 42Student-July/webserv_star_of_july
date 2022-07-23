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
  HeaderFieldParser hf_parser;
  StringVector headerfield_vec;

  for (std::string line = getLine(); line.size() != 0; line = getLine()) {
    headerfield_vec.push_back(line);
  }
  request->name_value_map = hf_parser.parse(headerfield_vec);
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
