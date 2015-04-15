#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include<string>


TEST(NginxConfigParserTest, SimpleConfigFromFile) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}


TEST(NginxConfigStatementTest, Tostring) {
  NginxConfigStatement statement;
  statement.tokens_push_back("foo");
  statement.tokens_push_back("bar");
  EXCEPT_EQ("foo bar;\n",statement.Tostring(0));
}

class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parserr_Parse(&config_stream, &config_);
	}

	NiginxCOnfigParser parser_;
	NiginxConfig config_;
};

TEST_F(NiginxStringConfigTest, SimpleTestConfig){
	EXCEPT_FAULSE(ParseString("foo bar"));
}

TEST_F(NiginxStringConfigTest, NestedTestConfig){
	EXCEPT_TRUE(ParseString(
		"foo { "
		"  bar { "
		"     bax qux;"
		"   }"
		"}"));
} 