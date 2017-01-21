#include "gtest/gtest.h"
#include "config_parser.h"

#include <sstream>
#include <string>
#include <iostream>

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

//Tests ToString
TEST(NginxConfigTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

class NginxStringConfigTest: public ::testing::Test {
protected:
	bool ParseString(const std::string& config_string){
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}

	NginxConfigParser parser_;
	NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, SimpleValidConfig){
	EXPECT_TRUE(ParseString("foo bar;"));
}

TEST_F(NginxStringConfigTest, NoSemiColonConfig){
	EXPECT_FALSE(ParseString("foo bar"));
}

//Multiple tests for the unbalanced brackets
TEST_F(NginxStringConfigTest, UnbalancedBracksConfig) {
	EXPECT_FALSE(ParseString("listen { foo bar;"));
	EXPECT_FALSE(ParseString("listen foo bar; }"));
	EXPECT_FALSE(ParseString("listen { foor bar; }}"));
	EXPECT_FALSE(ParseString("{ listen { foo bar; }"));
}

TEST_F(NginxStringConfigTest, BalancedConfig){
	EXPECT_TRUE(ParseString("server { listen 80; }"));
}


TEST_F(NginxStringConfigTest, NestedConfig){
	EXPECT_TRUE(ParseString("first { second { center; } }"));
}

TEST_F(NginxStringConfigTest, UnbalancedConfig){
	EXPECT_FALSE(ParseString("server { listen 80;"));
}


