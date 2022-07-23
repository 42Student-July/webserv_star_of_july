#include "HttpParser.hpp"

const std::string HttpParser::CRLF = "\r\n";

HttpParser::ParseErrorExeption::ParseErrorExeption(
    const std::string& error_status, const std::string& reason)
    : std::runtime_error(reason), error_status_(error_status) {}

HttpParser::ParseErrorExeption::~ParseErrorExeption() throw() {}

const std::string& HttpParser::ParseErrorExeption::getErrorStatus() const {
  return error_status_;
}

HttpParser::HttpParser() {}

HttpParser::~HttpParser() {}
