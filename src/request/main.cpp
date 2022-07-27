
#include <cstdlib>
#include <fstream>

#include "HttpRequest.hpp"
#include "HttpRequestConverter.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"

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

int main() {
  ServerConfig server_config;
  server_config.port = 4242;
  server_config.host = "42tokyo";
  server_config.server.push_back("nop");
  // error_pages_は省略
  server_config.root = "www/html";
  server_config.client_body_size_limit = 65536;
  // locations_は省略

  HttpRequestParser parser;
  std::string file_content =
      readFile("gtest/request/dto_one_header_field.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), server_config);

  std::cout << *request;
  std::cout << request->server_config.port << std::endl;
}
