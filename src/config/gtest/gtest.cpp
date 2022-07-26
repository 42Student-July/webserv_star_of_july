#include <gtest/gtest.h>

#include <stdexcept>

#include "../ConfigParser.hpp"

TEST(ConfigParserServersTests, CanParseHost) {
  ConfigParser configparser("./TestConfigs/listen.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("127.0.0.1", serverconfig[0].host);
}

TEST(ConfigParserServersTests, CanParsePort) {
  ConfigParser configparser("./TestConfigs/listen.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(4242, serverconfig[0].port);
}

TEST(ConfigParserServersTests, CanParseServerName) {
  ConfigParser configparser("./TestConfigs/server_name.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("hello", serverconfig[0].server[0]);
  ASSERT_EQ("hello2", serverconfig[0].server[1]);
  ASSERT_EQ("hello3", serverconfig[0].server[2]);
}

TEST(ConfigParserServersTests, CanParseRoot) {
  ConfigParser configparser("./TestConfigs/root.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/var/www/html", serverconfig[0].root);
}

TEST(ConfigParserServersTests, CanParseDualServers) {
  ConfigParser configparser("./TestConfigs/dual_servers.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(4242, serverconfig[0].port);
  ASSERT_EQ("a", serverconfig[0].server[0]);
  ASSERT_EQ("b", serverconfig[0].server[1]);
  ASSERT_EQ("c", serverconfig[0].server[2]);

  ASSERT_EQ(4343, serverconfig[1].port);
  ASSERT_EQ("/var/www/html", serverconfig[1].root);
}

TEST(ConfigParserServersTests, CanParseErrorPages) {
  ConfigParser configparser("./TestConfigs/error_pages.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/404.html", serverconfig[0].error_pages[404]);
}

TEST(ConfigParserServersTests, CanParseDualErrorPages) {
  ConfigParser configparser("./TestConfigs/dual_error_pages.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/404.html", serverconfig[0].error_pages[404]);
  ASSERT_EQ("/500.html", serverconfig[0].error_pages[500]);
}

TEST(ConfigParserServersTests, CanParseClientBodySizeLimit) {
  ConfigParser configparser("./TestConfigs/client_max_body_limit.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(1024, serverconfig[0].client_body_size_limit);
}

TEST(ConfigParserLocationsTests, CanParseLocationUri) {
  ConfigParser configparser("./TestConfigs/location.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/", serverconfig[0].locations[0].location);
}

TEST(ConfigParserLocationsTests, CanParseLocationRoot) {
  ConfigParser configparser("./TestConfigs/location_root.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("/var/www/html/locationroot", serverconfig[0].locations[0].root);
}

TEST(ConfigParserLocationsTests, CanParseLocationAllowedMethod) {
  ConfigParser configparser("./TestConfigs/location_allowed_method.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("GET", serverconfig[0].locations[0].allowed_methods[0]);
  ASSERT_EQ("POST", serverconfig[0].locations[0].allowed_methods[1]);
  ASSERT_EQ("DELETE", serverconfig[0].locations[0].allowed_methods[2]);
}

TEST(ConfigParserLocationsTests, CanParseLocationIndexes) {
  ConfigParser configparser("./TestConfigs/location_indexes.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("index.html", serverconfig[0].locations[0].indexes[0]);
  ASSERT_EQ("index.php", serverconfig[0].locations[0].indexes[1]);
}

TEST(ConfigParserLocationsTests, CanParseLocationAutoindex) {
  ConfigParser configparser("./TestConfigs/location_autoindex.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(1, serverconfig[0].locations[0].autoindex);
  ASSERT_EQ(0, serverconfig[0].locations[1].autoindex);
  ASSERT_EQ(1, serverconfig[1].locations[0].autoindex);
}

TEST(ConfigParserLocationsTests, CanParseLocationCGIPath) {
  ConfigParser configparser("./TestConfigs/location_cgipath.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ(".py", serverconfig[0].locations[0].cgi_extensions[0]);
}

TEST(ConfigParserLocationsTests, CanParseLocationRedirection)
{
  ConfigParser configparser("./TestConfigs/location_redirect.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("http://example.com/", serverconfig[0].locations[0].redirect[301]);
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

TEST(Error, DuplicatedListen) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/duplicated_listen.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/duplicated_listen.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Duplicated listen", e.what());
  }
}

TEST(Error, DuplicatedLocation) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/duplicated_location.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/duplicated_location.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Duplicated location", e.what());
  }
}

TEST(Error, DuplicatedLocation2) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/duplicated_location2.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/duplicated_location2.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Duplicated location", e.what());
  }
}

TEST(Error, InvalidAllowedMethod) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/invalid_allowed_method.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/invalid_allowed_method.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Invalid allowed_method", e.what());
  }
}

TEST(Error, InvalidAutoIndex) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/invalid_auto_index.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/invalid_auto_index.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Invalid autoindex", e.what());
  }
}

TEST(Error, InvalidErrorPages) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/invalid_error_pages.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/invalid_error_pages.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Invalid error_page", e.what());
  }
}

TEST(Error, InvalidErrorPages2) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/invalid_error_pages2.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/invalid_error_pages2.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Invalid error_page", e.what());
  }
}

TEST(Error, InvalidRedirect) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/invalid_location_redirect.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/invalid_location_redirect.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Invalid return", e.what());
  }
}

TEST(Error, InvalidRedirect2) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/invalid_location_redirect2.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/invalid_location_redirect2.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Invalid return", e.what());
  }
}

TEST(Error, NoListen) {
  try {
    ASSERT_THROW(
        ConfigParser configparser(
            "./TestConfigs/ErrorCases/no_listen.conf"),
        std::runtime_error);
    ConfigParser configparser(
        "./TestConfigs/ErrorCases/no_listen.conf");
  } catch (std::exception &e) {
    ASSERT_STREQ("Error: Config: Need listen setting", e.what());
  }
}

TEST(ConfigParserLocationsTests, CanParseSaishoConf) {
  ConfigParser configparser("./TestConfigs/saisho_example.conf");
  std::vector<ServerConfig> serverconfig = configparser.getServerConfigs();
  ASSERT_EQ("127.0.0.1", serverconfig[0].host);
  ASSERT_EQ(4242, serverconfig[0].port);
  ASSERT_EQ("/var/www/html", serverconfig[0].root);
  ASSERT_EQ("/404.html", serverconfig[0].error_pages[404]);
  ASSERT_EQ(1024, serverconfig[0].client_body_size_limit);
  ASSERT_EQ("/", serverconfig[0].locations[0].location);
  ASSERT_EQ("www/html", serverconfig[0].locations[0].root);
}
