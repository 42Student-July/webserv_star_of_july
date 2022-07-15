#include "CGIParser.hpp"

const std::string CGIParser::CRLF = "\r\n";
const std::string CGIParser::WS = " \t";

CGIParser::CGIParser(std::string cgi_response) : cgi_response_(cgi_response) {}

CGIParser::~CGIParser() {}

void CGIParser::parse() {
  offset_ = cgi_response_;

  parseHeader();
  parseBody();
}

void CGIParser::parseHeader() {
  HeaderFieldMap name_value_map;
  std::string line;

  while (getLine(&line) && line.size() != 0) {
    std::string::size_type name_end = line.find_first_of(":");
    std::string name = line.substr(0, name_end);
    std::string::size_type value_begin =
        line.find_first_not_of(WS, name_end + 1);
    std::string value = line.substr(value_begin);

    name_value_map[name] = value;
  }
}

void CGIParser::parseBody() {
  std::string line;
  std::string body;

  while (getLine(&line) && line.size() != 0) {
    body += line;
    body += "\n";
  }
}

// よしえさんのrequestparserより
// Parserクラスとして抽象化したい
// 現在のオフセットから一行読み取る関数。読み取ったら改行の次の文字にoffsetを進める
bool CGIParser::getLine(std::string* line) {
  std::string::size_type n = offset_.find(CRLF);

  if (n == std::string::npos) {
    return false;
  }
  *line = offset_.substr(0, n);
  offset_ = offset_.substr(n + 2);
  return true;
}
