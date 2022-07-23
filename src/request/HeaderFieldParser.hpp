#ifndef HEADERFIELDPARSER_HPP
#define HEADERFIELDPARSER_HPP

#include "HttpParser.hpp"
#include "utility.hpp"

class HeaderFieldParser : public HttpParser {
 public:
  HeaderFieldParser();
  ~HeaderFieldParser();
  HeaderFieldMap parse(const std::vector<std::string>& header_field_vec);

 private:
  static const std::string WS;
  static const std::string Delimiters;

  HeaderFieldParser(const HeaderFieldParser& other);
  HeaderFieldParser& operator=(const HeaderFieldParser& other);
  static void handleSameFiledName(HeaderFieldPair* header_field_pair,
                                  const HeaderFieldMap& header_field_map);
  static HeaderFieldPair parseHeaderField(const std::string& line);
  static std::string parseFieldName(const std::string& line,
                                    StringPos colon_pos);
  static std::string parseFieldValue(const std::string& line,
                                     StringPos colon_pos);
  static void validateHeaderField(const std::string& field_name,
                                  const std::string& field_value);
  static void validateHeaderFields(const HeaderFieldMap& headers);
  static bool isOWS(int c);
  static bool isHeaderDelimiter(int c);
  static bool isHeaderTokenChar(int c);
  static bool isHeaderToken(const std::string& str);
};

#endif /* HEADERFIELDPARSER_HPP */
