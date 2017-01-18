#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigStatementTest, toString){
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string& config_string){
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxStringConfigTest, SimpleTextConfig){

	EXPECT_TRUE(ParseString("foo bar;"));

}


TEST(NginxConfigParserTest, SimpleBadTextConfig){
	const std::string config_text = "foo bar";
	std::stringstream config_stream(config_text);
	NginxConfigParser parser;
	NginxConfig config;
	EXPECT_FALSE(parser.Parse(&config_stream, &config));
	
}

TEST_F(NginxStringConfigTest,NestedConfig)
{
	EXPECT_TRUE(ParseString(
		"foo { "
		"    bar  "
		"         baz;"
		"       }"
		"}"));
}


TEST_F(NginxStringConfigTest, SimpleTextConfig2){

	EXPECT_TRUE(ParseString("server;"));

}


TEST_F(NginxStringConfigTest, SimpleTextConfig3){

	EXPECT_FALSE(ParseString("swag"));

}

TEST_F(NginxStringConfigTest, SimpleTextConfig4){

	EXPECT_FALSE(ParseString("foo bar;;;;;;;;;;;;;;;;;;;"));

}


TEST_F(NginxStringConfigTest, SimpleTextConfig5){

	EXPECT_TRUE(ParseString("server foo bar baz;"));

}