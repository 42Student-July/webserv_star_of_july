#include <gtest/gtest.h>

#include <fstream>

#include "test_helper.hpp"

void compareString(const std::string &expected, const std::string &actual) {
  ASSERT_STREQ(expected.c_str(), actual.c_str());
}

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

ServerConfig initServerCongig() {
  ServerConfig server_config;
  server_config.port_ = 4242;
  server_config.host_ = "42tokyo";
  server_config.names_.push_back("nop");
  server_config.names_.push_back("cluster");
  // error_pages_は省略
  server_config.root_ = "www/html";
  server_config.client_body_size_limit_ = 65536;
  // locations_は省略
  return server_config;
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequest *request) {
  compareString(method, request->method);
  compareString(uri, request->uri);
  compareString(version, request->version);
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequestDTO *dto) {
  compareString(method, dto->method);
  compareString(uri, dto->path);
  compareString(version, dto->version);
}

// Request用
void checkHeaderField(const std::string &name, const std::string &value,
                      const HttpRequest::HeaderFieldMap &name_value_map) {
  HttpRequest::HeaderFieldMap::const_iterator it = name_value_map.find(name);

  ASSERT_TRUE(it != name_value_map.end());
  compareString(value, it->second);
}

// DTO用
void checkHeaderField(const std::string &expected, const std::string &actual) {
  compareString(expected, actual);
}

// 形式そろえるために
void checkBody(const std::string &expected, const std::string &actual) {
  compareString(expected, actual);
}
