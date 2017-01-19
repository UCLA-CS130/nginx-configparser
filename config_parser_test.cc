#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) { //first is test cases, second is test
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");

  EXPECT_EQ(statement.ToString(0), "foo bar;\n"); // note that the 0 contains the indentation level
}

class NginxStringConfigTest : public ::testing::Test { //fixture, lets you define helper methods
protected:
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
  
  EXPECT_TRUE(ParseString("foo bar;")); // note that we can treat the expects as straems
  EXPECT_EQ(1, out_config_.statements_.size()) 
    << "Config has one statements";
  EXPECT_EQ(out_config_.statements_[0]->tokens_[0], "foo");
  EXPECT_EQ(out_config_.statements_[0]->tokens_[1], "bar");
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}


TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server { listen 80; }"));
  // TODO: Test the contents of out_config_;
}

TEST_F(NginxStringConfigTest, UnbalancedBraces) {
  EXPECT_FALSE(ParseString("server { listen 80; "));
  EXPECT_FALSE(ParseString("server listen 80; } "));
}

TEST_F(NginxStringConfigTest, SemicolonIssues) {
  EXPECT_FALSE(ParseString("server { listen 80; }; "));
  EXPECT_FALSE(ParseString("server { listen 80; } \n \"test\" "));
}

TEST_F(NginxStringConfigTest, UnbalancedQuotations) {
  EXPECT_FALSE(ParseString("\"bar; "));
  EXPECT_FALSE(ParseString("\"bar;\";\" "));
}

TEST_F(NginxStringConfigTest, MultipleBlockConfig) {
  EXPECT_TRUE(ParseString("foo \"bar\"; \n server { listen 80;} server2 { listen 80; }"));
}

TEST_F(NginxStringConfigTest, EmbeddedBlockConfig) {
  EXPECT_TRUE(ParseString("server { server2 { listen 80; } }"));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
  EXPECT_TRUE(ParseString(""));
}