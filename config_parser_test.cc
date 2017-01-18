#include "gtest/gtest.h"
#include "config_parser.h"
#include <string>
#include <sstream>

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

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

TEST_F(NginxStringConfigTest, NestedConfigs) {
	EXPECT_TRUE(ParseString("foo { bar { baz; } }"));
}

TEST_F(NginxStringConfigTest, ImproperlyNestedConfigs) {
	EXPECT_FALSE(ParseString("foo { bar { baz; } } }"));
}

TEST_F(NginxStringConfigTest, BlockLeftUnclosed) {
	EXPECT_FALSE(ParseString("foo { bar { baz; } "));
}

TEST_F(NginxStringConfigTest, MultipleTokensBeforeBlock) {
	EXPECT_TRUE(ParseString("foo bar { baz; }"));
}




