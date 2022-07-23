#include "HttpRequestParser.hpp"
#include "utility.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

// 2つの引数はコンストラクタで渡した方が読みやすいかも。
HttpRequest *HttpRequestParser::parse(const char *buffer,
                                      const ServerConfig &server_config) {
  HttpRequest *req = new HttpRequest(server_config);
  StringPos offset = 0;

  try {
    req->request_line = parseRequestLine(buffer, &offset);
    req->name_value_map = parseHeaderField(buffer, &offset);
    req->body = parseBody(buffer, offset);
  } catch (const ParseErrorExeption &e) {
    req->response_status_code = e.getErrorStatus();
    std::cerr << e.what() << std::endl;
  } catch (const std::exception &e) {
    req->response_status_code = HttpStatus::INTERNAL_SERVER_ERROR;
    std::cerr << e.what() << std::endl;
  }
  return req;
}

RequestLine HttpRequestParser::parseRequestLine(const std::string &buffer,
                                                StringPos *offset) {
  RequestLineParser rl_parser;
  RequestLine request_line = rl_parser.parse(getLine(buffer, offset));

  return request_line;
}

HttpRequest::HeaderFieldMap HttpRequestParser::parseHeaderField(
    const std::string &buffer, StringPos *offset) {
  StringVector headerfield_vec;
  HeaderFieldParser hf_parser;
  HeaderFieldMap headerfield_map;

  for (std::string line = getLine(buffer, offset); line.size() != 0;
       line = getLine(buffer, offset)) {
    headerfield_vec.push_back(line);
  }
  headerfield_map = hf_parser.parse(headerfield_vec);
  return headerfield_map;
}

std::string HttpRequestParser::parseBody(const std::string &buffer,
                                         StringPos offset) {
  std::string body;
  // bodyがないケース
  if (offset == buffer.size()) {
    return "";
  }
  for (std::string line = getLine(buffer, &offset); line.size() != 0;
       line = getLine(buffer, &offset)) {
    body += line;
    body += "\n";
  }
  return body;
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
