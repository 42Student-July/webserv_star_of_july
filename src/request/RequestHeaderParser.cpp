#include "RequestHeaderParser.hpp"
#include "utility.hpp"

RequestHeaderParser::RequestHeaderParser() {}

RequestHeaderParser::~RequestHeaderParser() {}

RequestHeader RequestHeaderParser::parse(const std::string &unparsed_str) {
  RequestHeader header;
  StringPos offset = 0;

  validateRequestLength(unparsed_str);
  header.request_line = parseRequestLine(unparsed_str, &offset);
  header.name_value_map = parseHeaderField(unparsed_str, &offset);
  return header;
  //　try-catchは呼び出し元でやる(暫定)
  //
  // try {
  //   validateRequestLength(unparsed_str);
  //   header.request_line = parseRequestLine(unparsed_str, &offset);
  //   header.name_value_map = parseHeaderField(unparsed_str, &offset);
  // } catch (const ParseErrorExeption &e) {
  //   std::cerr << e.what() << std::endl;
  //   throw ParseErrorExeption(e.getErrorStatus(), "No header_end");
  // } catch (const std::exception &e) {
  //   std::cerr << e.what() << std::endl;
  //   throw ParseErrorExeption(HttpStatus::INTERNAL_SERVER_ERROR,
  //                            "No header_end");
  // }
  // return header;
}

// http://nginx.org/en/docs/http/ngx_http_core_module.html#client_header_buffer_size
void RequestHeaderParser::validateRequestLength(
    const std::string &unparsed_str) {
  size_t header_len = unparsed_str.size();
  if (header_len == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No header_end");
  }
  if (header_len > kMaxHeaderLength) {
    throw ParseErrorExeption(HttpStatus::REQUEST_HEADER_FIELD_TOO_LARGE,
                             "request is too long");
  }
}

RequestLine RequestHeaderParser::parseRequestLine(
    const std::string &unparsed_str, StringPos *offset) {
  RequestLineParser rl_parser;
  RequestLine request_line = rl_parser.parse(getLine(unparsed_str, offset));

  return request_line;
}

HeaderFieldMap RequestHeaderParser::parseHeaderField(
    const std::string &unparsed_str, StringPos *offset) {
  StringVector headerfield_vec;
  HeaderFieldParser hf_parser;
  HeaderFieldMap headerfield_map;

  for (std::string line = getLine(unparsed_str, offset); line.size() != 0;
       line = getLine(unparsed_str, offset)) {
    headerfield_vec.push_back(line);
  }
  headerfield_map = hf_parser.parse(headerfield_vec);
  return headerfield_map;
}

// 変数宣言と初期化を同時にするとなんか読みにくい。
// 現在のオフセットから一行読み取る関数。読み取ったら次の行頭にoffsetを進める
std::string RequestHeaderParser::getLine(const std::string &unparsed_str,
                                         StringPos *offset) {
  StringPos crlf_pos = unparsed_str.find(CRLF, *offset);
  if (crlf_pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "getLine() error");
  }
  std::string line = unparsed_str.substr(*offset, crlf_pos - *offset);
  *offset = crlf_pos + 2;
  return line;
}
