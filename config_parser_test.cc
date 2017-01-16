#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"





// NginxConfigStatement
class NginxConfigStatementTest : public ::testing::Test {
protected:
    
    NginxConfigStatement statement_;
    NginxConfig out_config_;
};

// tests functions for NginxConfigStatement
TEST_F(NginxConfigStatementTest, ToString){
    statement_.tokens_.push_back("foo");
    statement_.tokens_.push_back("bar");
    EXPECT_EQ(statement_.ToString(0), "foo bar;\n");
}

// NginxConfigParser
class NginxConfigParserTest : public ::testing::Test {
protected:
	bool ParseString(const std::string config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}
	NginxConfigParser parser_;
	NginxConfig out_config_;
};

// tests functions for NginxConfigParser::ToString
TEST_F(NginxConfigParserTest, SimpleConfig){
    bool success = parser_.Parse("example_config", &out_config_);
    EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, AnotherSimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, out_config_.statements_.size())
		<< "Config has one statements";
	EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigParserTest, InvalidConfig) {
	EXPECT_FALSE(ParseString("foo bar"));
}

// this test fails
TEST_F(NginxConfigParserTest, NestedConfig) {
	EXPECT_TRUE(ParseString("server { location {localhost 127.0.0.1;} }"));
}

// this test fails
TEST_F(NginxConfigParserTest, UnbalancedCurlyBrace){
    EXPECT_FALSE(ParseString("server listen 80;}"));
    EXPECT_FALSE(ParseString("{server listen 80;"));
}



