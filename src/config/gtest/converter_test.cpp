#include <gtest/gtest.h>

#include "ConfigConverter.hpp"
#include "ConfigDTO.hpp"
#include "ServerConfig.hpp"

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

TEST(ConfigConverterTest, StoreEverything) {
  ServerConfig config = initServerConfigWithLocation();
  ConfigConverter converter;
  ConfigDTO *dto = converter.toDTO(config);

  EXPECT_EQ("4242", dto->port);
  EXPECT_EQ("42tokyo", dto->host);
  EXPECT_EQ("nop", dto->server);
  EXPECT_EQ("error0", dto->error_pages[0]);
  EXPECT_EQ("www/html", dto->root);
  EXPECT_EQ("location", dto->locations[0].location);
  EXPECT_EQ("www/cgi-bin", dto->locations[0].root);
  EXPECT_EQ("GET", dto->locations[0].allowed_methods[0]);
  EXPECT_EQ("index.cgi", dto->locations[0].indexes[0]);
  EXPECT_EQ(false, dto->locations[0].autoindex);
  EXPECT_EQ(".py", dto->locations[0].cgi_extensions[0]);
}

TEST(ConfigConverterTest, StoreNoServerNameConfig) {
  ServerConfig config = initServerConfigWithLocation();
  config.server.clear();
  ConfigConverter converter;
  ConfigDTO *dto = converter.toDTO(config);

  EXPECT_EQ("4242", dto->port);
  EXPECT_EQ("42tokyo", dto->host);
  EXPECT_TRUE(dto->server.empty());
  EXPECT_EQ("error0", dto->error_pages[0]);
  EXPECT_EQ("www/html", dto->root);
  EXPECT_EQ("location", dto->locations[0].location);
  EXPECT_EQ("www/cgi-bin", dto->locations[0].root);
  EXPECT_EQ("GET", dto->locations[0].allowed_methods[0]);
  EXPECT_EQ("index.cgi", dto->locations[0].indexes[0]);
  EXPECT_EQ(false, dto->locations[0].autoindex);
  EXPECT_EQ(".py", dto->locations[0].cgi_extensions[0]);
}
