#include "CGIParser.hpp"

const std::string CGIParser::CRLF = "\r\n";
const std::string CGIParser::WS = " \t";

CGIParser::CGIParser() : raw_buffer_(""), offset_(0) {}

CGIParser::~CGIParser() {}

std::string CGIParser::getBodyStr() const { return res_body_str_; }

HttpResponseHeaderDTO CGIParser::getHeader() const { return header_; }

std::map<std::string, std::string> CGIParser::getCGIHeader() const {
  return cgi_header_;
}

void CGIParser::parse(std::string cgi_response) {
  cgi_response_ = cgi_response;
  raw_buffer_ = cgi_response_;
  std::stringstream ss(cgi_response_);

  parseHeaderField(ss);
  parseBody(ss);
}

void CGIParser::parseHeaderField(std::stringstream &ss) {
	std::string line;
	while (std::getline(ss, line, '\n')) {
		if (!line.size()) {
			break;
		}
		std::pair<std::string, std::string> name_value_pair = makeHeaderFieldPair(line);
		cgi_header_.insert(name_value_pair);
	}
}

std::pair<std::string, std::string> CGIParser::makeHeaderFieldPair(
    const std::string& line) {
  StringPos name_end = line.find_first_of(":");
  std::string name = line.substr(0, name_end);
  std::string value = line.substr(name_end + 2);

  return std::pair<std::string, std::string>(name, value);
}

void CGIParser::parseBody(std::stringstream &ss) {
  std::string line;
  while (std::getline(ss, line, '\n')) {
    res_body_str_ += line;
    res_body_str_ += "\n";
  }
}

// よしえさんのrequestparserより
// Parserクラスとして抽象化したい
// 現在のオフセットから一行読み取る関数。読み取ったら改行の次の文字にoffsetを進める
std::string CGIParser::getLine() {
  StringPos crlf_pos = raw_buffer_.find(CRLF, offset_);
  if (crlf_pos == std::string::npos) {
    throw -1;
  }
  std::string line = raw_buffer_.substr(offset_, crlf_pos - offset_);
  offset_ = crlf_pos + 2;
  return line;
}

