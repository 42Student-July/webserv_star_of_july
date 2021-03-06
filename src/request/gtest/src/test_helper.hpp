#ifndef TEST_HELPER_HPP
#define TEST_HELPER_HPP

#include "ConfigDTO.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestConverter.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "RequestHeaderParser.hpp"
#include "ServerConfig.hpp"
#include "WebservConfig.hpp"

const std::string file_dir = "gtest/request/";

ServerConfig initServerConfigWithLocation();
std::string readFile(const char *filepath);
HttpRequest *buildRequest(const std::string &filepath, size_t port,
                          const WebservConfig config);
HttpRequest *buildRequest2(const std::string &filepath,
                           const ServerConfig config);
RequestHeader buildRequestHeader(const std::string &filepath);
HttpRequestDTO *buildDTO(const std::string &filepath, size_t port,
                         const WebservConfig config);
void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequest *request);
void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, HttpRequestDTO *dto);
void checkRequestline(const std::string &method, const std::string &uri,
                      const std::string &version, RequestLine request_line);
void checkHeaderField(const std::string &name, const std::string &value,
                      const HeaderFieldMap &name_value_map);
void checkHeaderField(const std::string &actual, const std::string &expected);
void checkBody(const std::string &expected, const std::string &actual);

#endif /* TEST_HELPER_HPP */
