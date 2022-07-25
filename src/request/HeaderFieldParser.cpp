#include "HeaderFieldParser.hpp"

HeaderFieldParser::HeaderFieldParser() {}

HeaderFieldParser::~HeaderFieldParser() {}

// HeaderFieldPairを使ってるせいで可読性低い。けど同じヘッダーフィールドに対応すると、こうしないとfor文が長くなって微妙になった
HttpRequest::HeaderFieldMap HeaderFieldParser::parse(
    const std::vector<std::string>& header_field_vec) {
  HeaderFieldMap header_field_map;
  HeaderFieldPair header_field_pair;

  for (StringVector::const_iterator it = header_field_vec.begin();
       it != header_field_vec.end(); ++it) {
    header_field_pair = parseHeaderField(*it);
    handleSameFiledName(&header_field_pair, header_field_map);
    header_field_map[header_field_pair.first] = header_field_pair.second;
  }
  validateAllHeaderFields(header_field_map);
  return header_field_map;
}

HttpRequest::HeaderFieldPair HeaderFieldParser::parseHeaderField(
    const std::string& line) {
  StringPos colon_pos = line.find_first_of(":");
  if (colon_pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "header has no colon");
  }
  std::string field_name = parseFieldName(line, colon_pos);
  std::string field_value = parseFieldValue(line, colon_pos);

  validateOneHeaderField(field_name, field_value);
  return HeaderFieldPair(field_name, field_value);
}

// field nameは小文字で管理する。大文字小文字を区別しないため。
// rfc7230 3.2
// Each header field consists of a case-insensitive field name...
std::string HeaderFieldParser::parseFieldName(const std::string& line,
                                              StringPos colon_pos) {
  std::string raw_field_name = line.substr(0, colon_pos);

  return utility::toLower(raw_field_name);
}

std::string HeaderFieldParser::parseFieldValue(const std::string& line,
                                               StringPos colon_pos) {
  return utility::trimCopyIf(line.substr(colon_pos + 1), OWS);
}

void HeaderFieldParser::handleSameFiledName(
    HeaderFieldPair* header_field_pair,
    const HeaderFieldMap& header_field_map) {
  std::string target_field_name = header_field_pair->first;
  HeaderFieldMap::const_iterator it = header_field_map.find(target_field_name);

  if (it != header_field_map.end()) {
    if (target_field_name == "host") {
      throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "header has two Host");
    }
    std::string old_field_value = it->second;
    std::string new_field_value =
        old_field_value + "," + header_field_pair->second;
    header_field_pair->second = new_field_value;
  };
}

void HeaderFieldParser::validateOneHeaderField(const std::string& field_name,
                                               const std::string& field_value) {
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
  (void)field_value;
}

void HeaderFieldParser::validateAllHeaderFields(const HeaderFieldMap& headers) {
  HeaderFieldMap::const_iterator it = headers.find("host");

  if (it == headers.end() || it->second.empty()) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "no host");
  }
}
