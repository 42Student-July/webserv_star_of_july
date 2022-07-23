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
  HeaderFieldParser(const HeaderFieldParser& other);
  HeaderFieldParser& operator=(const HeaderFieldParser& other);
  static void handleSameFiledName(HeaderFieldPair* header_field_pair,
                                  const HeaderFieldMap& header_field_map);
  static HeaderFieldPair parseHeaderField(const std::string& line);
  static std::string parseFieldName(const std::string& line,
                                    StringPos colon_pos);
  static std::string parseFieldValue(const std::string& line,
                                     StringPos colon_pos);
  static void validateOneHeaderField(const std::string& field_name,
                                     const std::string& field_value);
  static void validateAllHeaderFields(const HeaderFieldMap& headers);
};

#endif /* HEADERFIELDPARSER_HPP */
