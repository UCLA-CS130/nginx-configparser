#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigStatementTest, ToString) {
    NginxConfigStatement statement;
    statement.tokens_.push_back("foo");
    statement.tokens_.push_back("bar");
    EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

TEST(NginxConfigStatementTest, SimpleTextConfig) {
    const std::string config_text = "foo bar;";
    std::stringstream config_stream(config_text);
    NginxConfigParser parser;
    NginxConfig config;
    EXPECT_TRUE(parser.Parse(&config_stream, &config));
}


TEST(NginxConfigParserTest, hw2_test_config) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("hw2_test_config", &out_config);

  EXPECT_TRUE(success);
}


