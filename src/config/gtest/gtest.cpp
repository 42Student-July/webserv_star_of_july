#include <gtest/gtest.h>

#include <stdexcept>

#include "../ConfigParser.hpp"

TEST(ConfigParserServersTests, CanParseHost) {
  ConfigParser configparser("./TestConfigs/listen.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("127.0.0.1", serverconfig[0].host_);
}

TEST(ConfigParserServersTests, CanParsePort) {
  ConfigParser configparser("./TestConfigs/listen.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(4242, serverconfig[0].port_);
}

TEST(ConfigParserServersTests, CanParseServerName) {
  ConfigParser configparser("./TestConfigs/server_name.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("hello", serverconfig[0].names_[0]);
  ASSERT_EQ("hello2", serverconfig[0].names_[1]);
  ASSERT_EQ("hello3", serverconfig[0].names_[2]);
}

TEST(ConfigParserServersTests, CanParseRoot) {
  ConfigParser configparser("./TestConfigs/root.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/var/www/html", serverconfig[0].root_);
}

TEST(ConfigParserServersTests, CanParseDualServers) {
  ConfigParser configparser("./TestConfigs/dual_servers.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(4242, serverconfig[0].port_);
  ASSERT_EQ("a", serverconfig[0].names_[0]);
  ASSERT_EQ("b", serverconfig[0].names_[1]);
  ASSERT_EQ("c", serverconfig[0].names_[2]);

  ASSERT_EQ(4343, serverconfig[1].port_);
  ASSERT_EQ("/var/www/html", serverconfig[1].root_);
}

TEST(ConfigParserServersTests, CanParseErrorPages) {
  ConfigParser configparser("./TestConfigs/error_pages.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/404.html", serverconfig[0].error_pages_[404]);
}

TEST(ConfigParserServersTests, CanParseDualErrorPages) {
  ConfigParser configparser("./TestConfigs/dual_error_pages.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/404.html", serverconfig[0].error_pages_[404]);
  ASSERT_EQ("/500.html", serverconfig[0].error_pages_[500]);
}

TEST(ConfigParserServersTests, CanParseClientBodySizeLimit) {
  ConfigParser configparser("./TestConfigs/client_max_body_limit.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(1024, serverconfig[0].client_body_size_limit_);
}

TEST(ConfigParserLocationsTests, CanParseLocationUri) {
  ConfigParser configparser("./TestConfigs/location.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/", serverconfig[0].locations_[0].name_);
}

TEST(ConfigParserLocationsTests, CanParseLocationRoot) {
  ConfigParser configparser("./TestConfigs/location_root.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/var/www/html/locationroot", serverconfig[0].locations_[0].root);
}

TEST(ConfigParserLocationsTests, CanParseLocationAllowedMethod) {
  ConfigParser configparser("./TestConfigs/location_allowed_method.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("GET", serverconfig[0].locations_[0].allowed_methods[0]);
  ASSERT_EQ("POST", serverconfig[0].locations_[0].allowed_methods[1]);
  ASSERT_EQ("DELETE", serverconfig[0].locations_[0].allowed_methods[2]);
}

TEST(ConfigParserLocationsTests, CanParseLocationIndexes) {
  ConfigParser configparser("./TestConfigs/location_indexes.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("index.html", serverconfig[0].locations_[0].indexes[0]);
  ASSERT_EQ("index.php", serverconfig[0].locations_[0].indexes[1]);
}

TEST(ConfigParserLocationsTests, CanParseLocationAutoindex) {
  ConfigParser configparser("./TestConfigs/location_autoindex.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(1, serverconfig[0].locations_[0].autoindex);
  ASSERT_EQ(0, serverconfig[0].locations_[1].autoindex);
  ASSERT_EQ(1, serverconfig[1].locations_[0].autoindex);
}

TEST(ConfigParserLocationsTests, CanParseLocationCGIPath) {
  ConfigParser configparser("./TestConfigs/location_cgipath.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(".py", serverconfig[0].locations_[0].cgi_extentions[0]);
}

TEST(Error, NoCurlyBracketsThrowsException) {
  try {
    ASSERT_THROW(ConfigParser configparser(
                     "./TestConfigs/ErrorCases/no_curly_brackets.conf"),
                 std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/no_curly_brackets.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Need to be closed by brackets", e.what());
  }
}

TEST(Error, WrongSyntaxServerThrowsException) {
  try {
    ASSERT_THROW(ConfigParser configparser(
                     "./TestConfigs/ErrorCases/wrong_syntax_server.conf"),
                 std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/wrong_syntax_server.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Wrong syntax", e.what());
  }
}

TEST(Error, NoCurlyBracketsLocationThrowsException) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/no_curly_brackets_location.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/no_curly_brackets_location.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Wrong syntax", e.what());
  }
}
