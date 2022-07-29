#include "HttpParser.hpp"

const std::string HttpParser::CRLF = "\r\n";
const std::string HttpParser::OWS = " \t";
const std::string HttpParser::Delimiters = "\"(),/:;<=>?@[\\]{}";

HttpParser::ParseErrorExeption::ParseErrorExeption(
    const std::string& error_status, const std::string& reason)
    : std::runtime_error(reason), error_status_(error_status) {}

HttpParser::ParseErrorExeption::~ParseErrorExeption() throw() {}

const std::string& HttpParser::ParseErrorExeption::getErrorStatus() const {
  return error_status_;
}

HttpParser::HttpParser() {}

HttpParser::~HttpParser() {}

bool HttpParser::isOWS(int c) { return OWS.find(c) != std::string::npos; }

bool HttpParser::isHeaderDelimiter(int c) {
  return Delimiters.find(c) != std::string::npos;
}

bool HttpParser::isHeaderTokenChar(int c) {
  if (isHeaderDelimiter(c) || c == ' ') {
    return false;
  }
  return isprint(c);
}

bool HttpParser::isHeaderToken(const std::string& str) {
  return utility::all_of(str.begin(), str.end(), isHeaderTokenChar);
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
// 現在のオフセットから一行読み取る関数。読み取ったら次の行頭にoffsetを進める
std::string HttpParser::getLine(const std::string& unparsed_str,
                                StringPos* offset) {
  StringPos crlf_pos = unparsed_str.find(CRLF, *offset);
  if (crlf_pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "getLine() error");
  }
  std::string line = unparsed_str.substr(*offset, crlf_pos - *offset);
  *offset = crlf_pos + 2;
  return line;
}
