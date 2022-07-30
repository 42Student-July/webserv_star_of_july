#include <gtest/gtest.h>

#include <stdexcept>

#include "../ConfigParser.hpp"
#include "../WebservConfig.hpp"

// ServerConfigの==演算子をオーバーロードするとASSERT_EQ使える
// でもそのためにはLocationConfigもオーバーロードする必要あり
// locationsはチェックしない簡易チェックにします
bool dataNearlyEqual(const ServerConfig& config1, const ServerConfig& config2) {
  if (config1.port != config2.port) {
    return false;
  }
  if (config1.host != config2.host) {
    return false;
  }
  if (config1.server != config2.server) {
    return false;
  }
  if (config1.error_pages != config2.error_pages) {
    return false;
  }
  if (config1.root != config2.root) {
    return false;
  }
  if (config1.client_body_size_limit != config2.client_body_size_limit) {
    return false;
  }
  // 略
  // if (config1.locations != config2.locations) {
  //   return false;
  // }
  return true;
}

TEST(WebservConfigPortsTest, Handles1Server) {
  ConfigParser parser("./TestConfigs/listen.conf");
  WebservConfig config = parser.getWebservConfig();
  PortSet expected_set;
  expected_set.insert(4242);
  ASSERT_EQ(expected_set, config.ports());
}

TEST(WebservConfigPortsTest, Handles2Server) {
  ConfigParser parser("./TestConfigs/dual_servers.conf");
  WebservConfig config = parser.getWebservConfig();
  PortSet expected_set;
  expected_set.insert(4242);
  expected_set.insert(4343);
  ASSERT_EQ(expected_set, config.ports());
}

TEST(WebservConfigPortsTest, Handles2ServerUsingSamePort) {
  ConfigParser parser("./TestConfigs/dual_servers_have_different_name.conf");
  WebservConfig config = parser.getWebservConfig();
  PortSet expected_set;
  expected_set.insert(4242);
  ASSERT_EQ(expected_set, config.ports());
}

TEST(WebservConfigFindServerConfigTest, FindsConfig) {
  ConfigParser parser("./TestConfigs/listen.conf");
  WebservConfig config = parser.getWebservConfig();
  ServerConfig expected = parser.getServerConfigs()[0];
  ServerConfig actual = config.findServerConfig(4242, "hello");
  ASSERT_TRUE(dataNearlyEqual(expected, actual));
}
