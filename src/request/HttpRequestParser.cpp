#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

// googleテスト用の関数
const HttpRequestParser::HeaderFieldMap& HttpRequestParser::getHeaderFieldMap()
    const {
  return name_value_map_;
}

HttpRequestDTO* HttpRequestParser::parse(const char* request_str) {
  HttpRequestDTO* request = new HttpRequestDTO;
  request->is_bad_request = false;
  offset_ = request_str;

  parseRequestLine(request);
  parseHeaderField(request);
  return request;
}

void HttpRequestParser::parseRequestLine(HttpRequestDTO* request) {
  std::string line;

  if (!getLine(&line)) {
    return;
  }

  std::string::size_type method_end = line.find_first_of(" ");
  request->method = line.substr(0, method_end);

  std::string::size_type uri_begin = line.find_first_not_of(" ", method_end);
  std::string::size_type uri_end = line.find_last_of(" ");
  request->uri = line.substr(uri_begin, uri_end - uri_begin);
  request->version = line.substr(uri_end + 1);
}

void HttpRequestParser::parseHeaderField(HttpRequestDTO* request) {
  storeHeaderField();
  setHeaderField(request);
}

// ヘッダーフィールドをすべてパースして、メンバ変数として持っておく。
// responseモジュールに渡す項目はsetHeaderFieldで判断する
void HttpRequestParser::storeHeaderField() {
  std::string line;

  while (getLine(&line) && line.size() != 0) {
    std::string::size_type name_end = line.find_first_of(":");
    std::string name = line.substr(0, name_end);
    std::string::size_type value_begin =
        line.find_first_not_of(WS, name_end + 1);
    std::string value = line.substr(value_begin);

    name_value_map_[name] = value;
  }
}

// responseモジュールに渡す項目をDTOにいれる
void HttpRequestParser::setHeaderField(HttpRequestDTO* request) {
  if (name_value_map_.find("Host") != name_value_map_.end()) {
    request->host = name_value_map_["Host"];
  }
  if (name_value_map_.find("Connection") != name_value_map_.end()) {
    request->connection = name_value_map_["Connection"];
  }
}

// void HttpRequestParser::parseBody() {
//   std::string line;
//   std::string body;

//   while (getLine(&line) && line.size() != 0) {
//     body += line;
//     body += "\n";
//   };
//   current_request_->body = body;
// }

// 現在のオフセットから一行読み取る関数。読み取ったら改行の次の文字にoffsetを進める
bool HttpRequestParser::getLine(std::string* line) {
  std::string::size_type n = offset_.find(CRLF);

  if (n == std::string::npos) {
    return false;
  }
  *line = offset_.substr(0, n);
  offset_ = offset_.substr(n + 2);
  return true;
}

const std::string HttpRequestParser::CRLF = "\r\n";
const std::string HttpRequestParser::WS = " \t";
