#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToStringSimple) {
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


TEST_F(NginxStringConfigTest, SimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse successfully";
	EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement.";
	EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
	EXPECT_FALSE(ParseString("")) << "Got empty string wrong.";
}

TEST_F(NginxStringConfigTest, BadConfig) {
	EXPECT_FALSE(ParseString("foo bar")) 
		<< "Bad config was actually parsed successfully";
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
	EXPECT_TRUE(ParseString("foo { bar { baz qux; } }")) 
		<< "Nested Config parsed incorrectly.";
	EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, TripleNestedConfig) {
	EXPECT_TRUE(ParseString("foo { bar { baz { qux; } } }"))
		<< "Triple nested config parsed incorrectly.";
}

TEST_F(NginxStringConfigTest, TwoBlocksConfig) {
	EXPECT_TRUE(ParseString("foo { bar; } baz { qux; }"))
		<< "Triple nested config parsed incorrectly.";
		EXPECT_TRUE(ParseString("foo { bar { baz qux; } } baz { qux; }"))
		<< "Triple nested config parsed incorrectly.";
}