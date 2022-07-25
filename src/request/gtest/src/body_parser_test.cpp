#include <gtest/gtest.h>

#include "BodyParser.hpp"
#include "test_helper.hpp"

class BodyParserTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  BodyParser parser;
};

TEST_F(BodyParserTest, NoChunk) {
  std::string buffer = "12345\r\n";
  std::string body = parser.parse(buffer, false, 5);

  EXPECT_EQ("12345", body);
}

TEST_F(BodyParserTest, NoBody) {
  std::string buffer = "12345\r\n";
  std::string body = parser.parse(buffer, false, 0);

  EXPECT_EQ("", body);
}

TEST_F(BodyParserTest, BodyIsLessThanContentLength) {
  std::string buffer = "12345\r\n";
  EXPECT_THROW(parser.parse(buffer, false, 4), std::runtime_error);
}
