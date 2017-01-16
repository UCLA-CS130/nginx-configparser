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
// TODO: rename tests to be systematic and meaningful
TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size())
    << "Config should have exactly one statement";
  EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
}

TEST_F(NginxStringConfigTest, MissingSemicolon) {
  EXPECT_FALSE(ParseString("foo bar"));
}

// Single statement, but it should have an NginxConfig child_block_
TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server { listen 80; }"));
  EXPECT_EQ(1, out_config_.statements_.size());
  EXPECT_TRUE(out_config_.statements_[0]->child_block_.get() != nullptr)
    << "Child block should exist for nested NginxConfig";

  EXPECT_EQ(2, out_config_.statements_[0]->child_block_
                                         ->statements_[0]->tokens_.size())
    << "Child block should be parsed properly for nested NginxConfig";
}

// TODO: Unmatched curly braces {}: +1 for {, -1 for }
// TODO: Comments in a config file: # This is a comment
// TODO: 2 levels of nesting: foo { bar { choo }}
// TODO: There may be other bugs

