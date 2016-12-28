#include <string>
#include<sstream>

#include "gtest/gtest.h"
#include "config_parser.h"



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
	EXPECT_TRUE(ParseString("foo bar;")); 
};

/* tests to see if a string with out a semicolon is a bad string */
TEST_F(NginxStringConfigTest, BadConfig) {
	EXPECT_FALSE(ParseString("foobar"));
}

TEST_F(NginxStringConfigTest, BadEndBlockConfig) {
	EXPECT_FALSE(ParseString("hello};"));
}

TEST(NginxConfigParserTest, NormalConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;
  bool success = parser.Parse("nginx.conf", &out_config);

  EXPECT_TRUE(success);

}
