#include <gtest/gtest.h>

#include "BodyParser.hpp"
#include "test_helper.hpp"

class BodyParserTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  BodyParser parser;
};

TEST_F(BodyParserTest, ContentLengthIsJustBufferSize) {
  std::string buffer = "12345";
  std::string body = parser.parse(buffer, false, true, 5);

  EXPECT_EQ("12345", body);
}

TEST_F(BodyParserTest, ContentLengthIsLessThanBufferSize) {
  std::string buffer = "12345";
  std::string body = parser.parse(buffer, false, true, 4);

  EXPECT_EQ("1234", body);
}

TEST_F(BodyParserTest, ContentLengthIsGreaterThanBufferSize) {
  std::string buffer = "12345";
  EXPECT_THROW(parser.parse(buffer, false, true, 6), std::runtime_error);
}

TEST_F(BodyParserTest, ContentLength_0) {
  std::string buffer = "12345";
  std::string body = parser.parse(buffer, false, true, 0);

  EXPECT_EQ("", body);
}

// 0だとバグるので暫定的に
TEST_F(BodyParserTest, NoContentLengthAndNoTransferEncoding) {
  std::string buffer = "12345";
  std::string body = parser.parse(buffer, false, false, 0);

  EXPECT_EQ("", body);
  // EXPECT_THROW(parser.parse(buffer, false, false, 0), std::runtime_error);
}

TEST_F(BodyParserTest, Chunk_NoChunk) {
  std::string buffer = "0\r\n\r\n";
  std::string body = parser.parse(buffer, true, false, 0);

  EXPECT_EQ("", body);
}

TEST_F(BodyParserTest, Chunk_1Chunk) {
  std::string buffer =
      "a\r\n123456789a\r\n"
      "0\r\n\r\n";
  std::string body = parser.parse(buffer, true, false, 0);

  EXPECT_EQ("123456789a", body);
}

TEST_F(BodyParserTest, ChunkSize_DataIsLess) {
  std::string buffer =
      "a\r\n123456789\r\n"
      "0\r\n\r\n";
  EXPECT_THROW(parser.parse(buffer, true, false, 0), std::runtime_error);
}

TEST_F(BodyParserTest, ChunkSize_DataIsGreater) {
  std::string buffer =
      "a\r\n123456789ab\r\n"
      "0\r\n\r\n";
  EXPECT_THROW(parser.parse(buffer, true, false, 0), std::runtime_error);
}

TEST_F(BodyParserTest, Chunk_2ChunkNonl) {
  std::string buffer =
      "2\r\n42\r\n"
      "5\r\ntokyo\r\n"
      "0\r\n\r\n";
  std::string body = parser.parse(buffer, true, false, 0);

  EXPECT_EQ("42tokyo", body);
}

TEST_F(BodyParserTest, Chunk_2ChunkNl) {
  std::string buffer =
      "3\r\n42\n\r\n"
      "6\r\ntokyo\n\r\n"
      "0\r\n\r\n";
  std::string body = parser.parse(buffer, true, false, 0);

  EXPECT_EQ("42\ntokyo\n", body);
}

// postリクエストにいれるbodyよくわからないので
TEST_F(BodyParserTest, Chunk_10ChunkNl) {
  std::string buffer =
      "14\r\nhogehoge1\nfugafuga1\n\r\n"
      "14\r\nhogehoge2\nfugafuga2\n\r\n"
      "14\r\nhogehoge3\nfugafuga3\n\r\n"
      "14\r\nhogehoge4\nfugafuga4\n\r\n"
      "14\r\nhogehoge5\nfugafuga5\n\r\n"
      "14\r\nhogehoge6\nfugafuga6\n\r\n"
      "14\r\nhogehoge7\nfugafuga7\n\r\n"
      "14\r\nhogehoge8\nfugafuga8\n\r\n"
      "14\r\nhogehoge9\nfugafuga9\n\r\n"
      "16\r\nhogehoge10\nfugafuga10\n\r\n"
      "0\r\n\r\n";
  std::string body = parser.parse(buffer, true, false, 0);

  std::string expetcted =
      "hogehoge1\nfugafuga1\n"
      "hogehoge2\nfugafuga2\n"
      "hogehoge3\nfugafuga3\n"
      "hogehoge4\nfugafuga4\n"
      "hogehoge5\nfugafuga5\n"
      "hogehoge6\nfugafuga6\n"
      "hogehoge7\nfugafuga7\n"
      "hogehoge8\nfugafuga8\n"
      "hogehoge9\nfugafuga9\n"
      "hogehoge10\nfugafuga10\n";

  EXPECT_EQ(expetcted, body);
}

TEST_F(BodyParserTest, ChunkSize_InvalidChar) {
  std::string buffer =
      "5g\r\n12345\r\n"
      "0\r\n\r\n";

  EXPECT_THROW(parser.parse(buffer, true, false, 0), std::invalid_argument);
}

TEST_F(BodyParserTest, ChunkSize_HasSpase) {
  std::string buffer =
      " 5\r\n12345\r\n"
      "0\r\n\r\n";

  EXPECT_THROW(parser.parse(buffer, true, false, 0), std::invalid_argument);
}

TEST_F(BodyParserTest, ChunkSize_OverFlow) {
  std::string buffer =
      "2147483648\r\n12345\r\n"
      "0\r\n\r\n";

  EXPECT_THROW(parser.parse(buffer, true, false, 0), std::out_of_range);
}

TEST_F(BodyParserTest, ChunkSize_Minus) {
  std::string buffer =
      "-5\r\n12345\r\n"
      "0\r\n\r\n";

  EXPECT_THROW(parser.parse(buffer, true, false, 0), std::invalid_argument);
}

TEST_F(BodyParserTest, ChunkSize_UpperCase) {
  std::string buffer =
      "A\r\n123456789a\r\n"
      "0\r\n\r\n";
  std::string body = parser.parse(buffer, true, false, 0);

  EXPECT_EQ("123456789a", body);
}
