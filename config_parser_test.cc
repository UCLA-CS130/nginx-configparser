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

TEST(NginxConfigTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ(statement.ToString(0), "foo bar;\n");
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

TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, out_config_.statements_.size())
		<< "Config has one statements";
	EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
	EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, ConfigComment) {
	EXPECT_TRUE(ParseString("worker_connections 768; #user www-data;"));
	EXPECT_EQ(1, out_config_.statements_.size());
}

TEST_F(NginxStringConfigTest, NestedConfig) {
	EXPECT_TRUE(ParseString("server { listen 80; }"));
	
}

TEST_F(NginxStringConfigTest, InvalidNestedConfigEnd) {
	EXPECT_FALSE(ParseString("server { listen 80; };"));
}

TEST_F(NginxStringConfigTest, InvalidNestedConfigStatement) {
	EXPECT_FALSE(ParseString("server { listen 80 }"));

}

/*

server {
        	cc {
        		root /data/www;
    		}
			location {
        		root /data;
    		}
}

This is totally valid, but initially the parser fails to parse this. 
The error was "Bad transition from TOKEN_TYPE_END_BLOCK to TOKEN_TYPE_END_BLOCK"
It passes this test case after the bug fix.

*/

TEST_F(NginxStringConfigTest,NestedConfigMultiple){
	const char *text =
	"server {"
        	"cc {"
        		"root /data/www;"
    		"}"
		"location {"
        		"root /data;"
    		"}"
        "}";
	std::string test(text);
	EXPECT_TRUE(ParseString(test));
}



TEST_F(NginxStringConfigTest,NestedConfigContent){
	const char *text =
	"server {"
        "listen 80 default_server;"
        "server_name localhost;"
	"}";
	std::string test(text);
	EXPECT_TRUE(ParseString(test));
	EXPECT_EQ("server_name", out_config_.statements_.at(0)->child_block_->statements_.at(1)->tokens_.at(0));
	EXPECT_EQ("80", out_config_.statements_.at(0)->child_block_->statements_.at(0)->tokens_.at(1));
}


