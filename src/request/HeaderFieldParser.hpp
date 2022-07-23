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
  static HeaderFieldPair makeHeaderFieldPair(const std::string& line);
  static void validateHeaderField(const HeaderFieldPair& headerfield_pair);
  static void validateHeaderFields(const HeaderFieldMap& headers);
  static bool isOWS(int c);
  static bool isHeaderDelimiter(int c);
  static bool isHeaderTokenChar(int c);
  static bool isHeaderToken(const std::string& str);
};

#endif /* HEADERFIELDPARSER_HPP */
