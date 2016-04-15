#include <string>
#include <sstream>
#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest2, TestFoo) {
  std::stringstream config_stream("foo bar");
  NginxConfigParser parser;
  NginxConfig out;
  parser.Parse(&config_stream, &out);
}

TEST(NginxConfigParserTest2, TestFoo) {
  std::stringstream config_stream("foo { abc xyz; }");
  NginxConfigParser parser;
  NginxConfig out;
  parser.Parse(&config_stream, &out);
}

TEST(NginxConfigParserTest2, TestFoo) {
  std::stringstream config_stream("foo { abc xyz; a b; }");
  NginxConfigParser parser;
  NginxConfig out;
  parser.Parse(&config_stream, &out);
}

