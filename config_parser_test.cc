#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToStringTest) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

//Test Fixture for passing in a parsed string into the parse function
class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}
	NginxConfigParser parser_;
	NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, out_config_.statements_.size())
		<< "Config has one statements";
	EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
	EXPECT_EQ("bar", out_config_.statements_[0]->tokens_[1]);
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
    EXPECT_FALSE(ParseString(";"));
    EXPECT_FALSE(ParseString(""));
	EXPECT_FALSE(ParseString("server { }"));
}

TEST_F(NginxStringConfigTest, InvalidConfigStatement) {
	EXPECT_FALSE(ParseString("foo bar"));
	EXPECT_FALSE(ParseString("server { listen 80 }"));
}

TEST_F(NginxStringConfigTest, NestedConfigStatement) {
	EXPECT_FALSE(ParseString("server { listen 80; "));
	EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statement";
	EXPECT_EQ("server", out_config_.statements_[0]->tokens_[0]);
}
