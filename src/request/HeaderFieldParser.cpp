#include "HeaderFieldParser.hpp"

const std::string HeaderFieldParser::WS = " \t";
const std::string HeaderFieldParser::Delimiters = "\"(),/:;<=>?@[\\]{}";

HeaderFieldParser::HeaderFieldParser() {}

HeaderFieldParser::~HeaderFieldParser() {}

HttpRequest::HeaderFieldMap HeaderFieldParser::parse(
    const std::vector<std::string>& header_field_vec) {
  HeaderFieldMap header_field_map;

  for (StringVector::const_iterator it = header_field_vec.begin();
       it != header_field_vec.end(); ++it) {
    HeaderFieldPair name_value_pair = makeHeaderFieldPair(*it);

    validateHeaderField(name_value_pair);
    header_field_map.insert(name_value_pair);
  }
  validateHeaderFields(header_field_map);
  return header_field_map;
}

HttpRequest::HeaderFieldPair HeaderFieldParser::makeHeaderFieldPair(
    const std::string& line) {
  StringPos name_end = line.find_first_of(":");

  if (name_end == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "header has no colon");
  }

  std::string name = line.substr(0, name_end);
  std::string value = utility::trimCopyIf(line.substr(name_end + 1), WS);

  return HeaderFieldPair(name, value);
}

void HeaderFieldParser::validateHeaderField(
    const HeaderFieldPair& headerfield_pair) {
  std::string field_name = headerfield_pair.first;
  std::string field_value = headerfield_pair.second;

  if (field_name.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "header has no field name");
  }
  if (isOWS(field_name[field_name.size() - 1])) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "header has space before colon");
  }
  if (!isHeaderToken(field_name)) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "field_name is not header token");
  }
}

void HeaderFieldParser::validateHeaderFields(const HeaderFieldMap& headers) {
  HeaderFieldMap::const_iterator it = headers.find("Host");
  if (it == headers.end()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "no host");
  }
}

bool HeaderFieldParser::isOWS(int c) { return (c == ' ' || c == '\t'); }

bool HeaderFieldParser::isHeaderDelimiter(int c) {
  return Delimiters.find(c) != std::string::npos;
}

bool HeaderFieldParser::isHeaderTokenChar(int c) {
  if (isHeaderDelimiter(c) || c == ' ') {
    return false;
  }
  return isprint(c);
}

bool HeaderFieldParser::isHeaderToken(const std::string& str) {
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
