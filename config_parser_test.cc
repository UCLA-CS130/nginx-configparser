#include "gtest/gtest.h"
#include "config_parser.h"

// given SimpleConfig test

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}


// test fixture for strings

class NginxStringConfigTest : public ::testing::Test {
protected:
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}


// bracketing tests

TEST_F(NginxStringConfigTest, UnbalancedLeftConfig) {
  EXPECT_FALSE(ParseString("server { listen 80;"));
}

TEST_F(NginxStringConfigTest, UnbalancedRightConfig) {
  EXPECT_FALSE(ParseString("foo { bar serve; } }"));
}

TEST_F(NginxStringConfigTest, NestedBlocksConfig) {
  EXPECT_TRUE(ParseString("server { foo { listen 80; } }"));
}

TEST_F(NginxStringConfigTest, EmptyBlockConfig) {
  EXPECT_TRUE(ParseString("foo {  }"));
}


// comment tests

TEST_F(NginxStringConfigTest, StatementCommentConfig) {
  EXPECT_TRUE(ParseString("foo bar;#foo bar comment"));
}

TEST_F(NginxStringConfigTest, CommentStatementConfig) {
  EXPECT_TRUE(ParseString("#foo bar comment\nfoo bar;"));
}

TEST_F(NginxStringConfigTest, CommentNoConfig) {
  EXPECT_FALSE(ParseString("#lol"));
}


// quotes tests

TEST_F(NginxStringConfigTest, QuotesConfig) {
  EXPECT_TRUE(ParseString("foo \"bar\";"));
}

TEST_F(NginxStringConfigTest, UnmatchedQuoteConfig) {
  EXPECT_FALSE(ParseString("foo \"bar;"));
}

// .ToString test
TEST(NginxConfigTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("boo");
  statement.tokens_.push_back("far");
  EXPECT_EQ(statement.ToString(0), "boo far;\n");
}
