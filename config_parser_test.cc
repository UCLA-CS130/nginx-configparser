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

TEST(NginxConfigParserTest, BadConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("bad_example_config", &out_config);

  EXPECT_FALSE(success);
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

TEST_F(NginxStringConfigTest, SimpleStatement) {
	EXPECT_TRUE(ParseString("foo bar;"));
        EXPECT_EQ(1, out_config_.statements_.size());
	EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
}

TEST_F(NginxStringConfigTest, TwoStatementConfig) {
	EXPECT_TRUE(ParseString("foo bar; foo { bar; }"));
        EXPECT_EQ(2, out_config_.statements_.size());
	EXPECT_EQ("foo", out_config_.statements_[1]->tokens_[0]);
}

TEST_F(NginxStringConfigTest, ToString) {
	std::string sample_string = "foo bar; foo { bar; }";
	EXPECT_TRUE(ParseString(sample_string));
        EXPECT_EQ("foo bar;\n", out_config_.statements_[0]->ToString(0));
}


TEST_F(NginxStringConfigTest, SimpleStatementWithBlock) {
	EXPECT_TRUE(ParseString("foo { bar; }"));
}

TEST_F(NginxStringConfigTest, Comment) {
	EXPECT_TRUE(ParseString("foo; #hey\n bar;"));
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
	EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, EmptyBlock) {
	EXPECT_TRUE(ParseString("foo {} "));
}

TEST_F(NginxStringConfigTest, StartWithBlock) {
	EXPECT_FALSE(ParseString("{} "));
}

TEST_F(NginxStringConfigTest, BlockWithoutToken) {
	EXPECT_FALSE(ParseString("foo; {}"));
}

TEST_F(NginxStringConfigTest, MultipleStatements) {
	EXPECT_TRUE(ParseString("foo; bar { baz; }"));
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




