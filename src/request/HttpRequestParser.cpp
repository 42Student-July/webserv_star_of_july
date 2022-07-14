#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest* HttpRequestParser::parse(const char* request_str,
                                      const ServerConfig& server_config) {
  HttpRequest* request = new HttpRequest;
  request->is_bad_request = false;
  request->server_config = server_config;
  offset_ = request_str;

  parseRequestLine(request);
  parseHeaderField(request);
  parseBody(request);
  return request;
}

void HttpRequestParser::parseRequestLine(HttpRequest* request) {
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

void HttpRequestParser::parseHeaderField(HttpRequest* request) {
  HttpRequest::HeaderFieldMap name_value_map;
  std::string line;

  while (getLine(&line) && line.size() != 0) {
    std::string::size_type name_end = line.find_first_of(":");
    std::string name = line.substr(0, name_end);
    std::string::size_type value_begin =
        line.find_first_not_of(WS, name_end + 1);
    std::string value = line.substr(value_begin);

    name_value_map[name] = value;
  }
  request->name_value_map = name_value_map;
}

void HttpRequestParser::parseBody(HttpRequest* request) {
  std::string line;
  std::string body;

  while (getLine(&line) && line.size() != 0) {
    body += line;
    body += "\n";
  }
  request->body = body;
}

// responseモジュールに渡す項目をDTOにいれる
// void HttpRequestParser::setHeaderField(HttpRequest* request) {
//   if (name_value_map_.find("Host") != name_value_map_.end()) {
//     request->host = name_value_map_["Host"];
//   }
//   if (name_value_map_.find("Connection") != name_value_map_.end()) {
//     request->connection = name_value_map_["Connection"];
//   }
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
