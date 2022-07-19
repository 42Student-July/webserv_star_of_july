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
  server_config.port = 4242;
  server_config.host = "42tokyo";
  server_config.server.push_back("nop");
  server_config.server.push_back("cluster");
  // error_pages_は省略
  server_config.root = "www/html";
  server_config.client_body_size_limit = 65536;
  // locations_は省略
  return server_config;
}

ServerConfig initServerConfigWithLocation() {
  ServerConfig server_config;
  server_config.port = 4242;
  server_config.host = "42tokyo";
  server_config.server.push_back("nop");
  server_config.server.push_back("cluster");
  server_config.error_pages.insert({0, "error0"});
  server_config.error_pages.insert({1, "error1"});
  server_config.root = "www/html";
  server_config.client_body_size_limit = 65536;
  LocationConfig locationdemo;
  locationdemo.location = "location";
  locationdemo.root = "www/cgi-bin";
  locationdemo.allowed_methods.push_back("GET");
  locationdemo.indexes.push_back("index.cgi");
  locationdemo.autoindex = false;
  locationdemo.cgi_extensions.push_back(".py");
  server_config.locations.push_back(locationdemo);
  return server_config;
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequest *request) {
  ASSERT_EQ(method, request->method);
  ASSERT_EQ(uri, request->uri);
  ASSERT_EQ(version, request->version);
}

void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequestDTO *dto) {
  ASSERT_EQ(method, dto->method);
  ASSERT_EQ(uri, dto->path);
  ASSERT_EQ(version, dto->version);
}

// Request用
void checkHeaderField(const std::string &name, const std::string &value,
                      const HttpRequest::HeaderFieldMap &name_value_map) {
  HttpRequest::HeaderFieldMap::const_iterator it = name_value_map.find(name);

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
