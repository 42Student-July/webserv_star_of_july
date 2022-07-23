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
