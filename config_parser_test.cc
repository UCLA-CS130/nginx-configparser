#include <sstream>
#include <string>

#include "config_parser.h"
#include "gtest/gtest.h"

// NginxConfigParser parses a config file to produce an NginxConfig
// An NginxConfig consists of 1 or more NginxConfigStatement's,
// which may in turn have a nested NginxConfig.

// Test NginxConfigStatement::ToString()
TEST(NginxConfigTest, ToStringTokens) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

// Test NginxConfigParser::Parse() with config strings
// (as opposed to full-on files)
class NginxStringConfigTest : public ::testing::Test {
protected:
  // Helper method to pass arbitrary strings into the parser
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }

  NginxConfigParser parser_;
  NginxConfig out_config_;
};

// Check the internal representation of "foo bar;"
TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size())
    << "Config has one statement";
  EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
}

// Invalid config should fail (missing semicolon)
TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}

// TODO: Unbalanced {} should not parse
// There is at least 1 other bug
TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server { listen 80; }"));
  // TODO: Test the contents of out_config_;
}
