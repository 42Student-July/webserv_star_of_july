#include <gtest/gtest.h>

#include <fstream>

#include "test_helper.hpp"

std::string readFile(const char *filepath) {
  std::ifstream ifs(filepath);

  if (ifs.fail()) {
    std::cerr << "ifstream:open failed()";
    exit(EXIT_FAILURE);
  }
  std::string file_content((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());
  return file_content;
}

HttpRequest *buildRequest(const std::string &filepath, size_t port,
                          const WebservConfig config) {
  std::string content;
  HttpRequestParser parser;

  content = readFile(filepath.c_str());
  parser.parse(content.c_str(), port, config);
  return parser.buildRequest();
}

RequestHeader buildRequestHeader(const std::string &filepath) {
  std::string content;
  std::string unparsed_header;
  RequestHeaderParser parser;

  content = readFile(filepath.c_str());
  unparsed_header = content.substr(0, content.find("\r\n\r\n") + 4);
  return parser.parse(unparsed_header);
}

HttpRequestDTO *buildDTO(const std::string &filepath, size_t port,
                         const WebservConfig config) {
  HttpRequestConverter converter;
  HttpRequest *req;
  HttpRequestDTO *dto;

  req = buildRequest(filepath, port, config);
  dto = converter.toDTO(*req);
  delete req;
  return dto;
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequest *request) {
  const RequestLine &request_line = request->header.requestLine();
  ASSERT_EQ(method, request_line.method);
  ASSERT_EQ(uri, request_line.uri);
  ASSERT_EQ(version, request_line.version);
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, RequestLine request_line) {
  ASSERT_EQ(method, request_line.method);
  ASSERT_EQ(uri, request_line.uri);
  ASSERT_EQ(version, request_line.version);
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequestDTO *dto) {
  ASSERT_EQ(method, dto->method);
  ASSERT_EQ(uri, dto->path);
  ASSERT_EQ(version, dto->version);
}

// Request用
void checkHeaderField(const std::string &name, const std::string &value,
                      const HeaderFieldMap &name_value_map) {
  HeaderFieldMap::const_iterator it = name_value_map.find(name);

  ASSERT_TRUE(it != name_value_map.end());
  ASSERT_EQ(value, it->second);
}

// DTO用
void checkHeaderField(const std::string &expected, const std::string &actual) {
  ASSERT_EQ(expected, actual);
}

// 形式そろえるために
void checkBody(const std::string &expected, const std::string &actual) {
  ASSERT_EQ(expected, actual);
}
