#include "gtest/gtest.h"
#include "config_parser.h"

#include <sstream>
#include <string>

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, ToStringSimple) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ("foo bar;\n", statement.ToString(0)) << "foo bar wasn't serialized correctly";
}

class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig2) {
	EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse successfully";
	EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement.";
	EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
	EXPECT_FALSE(ParseString("")) << "Got empty string wrong";
}

TEST_F(NginxStringConfigTest, BadConfig) {
	EXPECT_FALSE(ParseString("foo bar")) << "Bad config was parsed successfully";
}

TEST_F(NginxStringConfigTest, NestedConfig) {
	EXPECT_TRUE(ParseString("server { listen 80; }")) << "Nested config parsed incorrectly.";
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
	// bug in code found and fixed
	EXPECT_TRUE(ParseString("foo { bar { baz aux; } }")) << "Double nested config parsed incorrectly.";
}

TEST_F(NginxStringConfigTest, WrongDoubleNestedConfig) {
	EXPECT_FALSE(ParseString("foo { bar { baz aux; }")) << "Wrong double nested config parsed successfully.";
}

TEST_F(NginxStringConfigTest, WrongDoubleNestedConfig2) {
	EXPECT_FALSE(ParseString("foo { bar baz aux; } }")) << "Second wrong double nested config parsed successfully.";
}

TEST_F(NginxStringConfigTest, NginxConfigFileTest) {

	bool success = parser_.Parse("nginx_config", &config_);

  	EXPECT_TRUE(success) << "nginx_config file did not parse successfully";
}


