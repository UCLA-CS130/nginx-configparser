#include "gtest/gtest.h"
#include "config_parser.h"
using namespace std;
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}
class NginxStringConfigTest:public ::testing::Test{
protected:
	bool ParseString(const string config_string){
		stringstream config_stream(config_string);
		return parser_.Parse(&config_stream,&out_config_);
	}
	NginxConfigParser parser_;
	NginxConfig out_config_;
};
TEST_F(NginxStringConfigTest,AnotherSimpleConfig){
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1,out_config_.statements_.size());
	EXPECT_EQ("foo",out_config_.statements_.at(0)->tokens_.at(0));
}
TEST_F(NginxStringConfigTest,InvalidConfig){
	EXPECT_FALSE(ParseString("foo bar"));
}
	
TEST_F(NginxStringConfigTest,UnbalancedBracket){//this test shows an error, the parser does not 
	//handle unbalanced bracket correctly
	bool success=ParseString("server { listen 80;");
	EXPECT_FALSE(success);
}

TEST_F(NginxStringConfigTest,MissingParameter){//this test shows an error, the parser does not 
	//handle missing parameters correctly

	EXPECT_FALSE(ParseString(" server { listen ;}"));
}

TEST_F(NginxStringConfigTest,ExtraParameter){//this test shows an error, the parser does not handle
	//extra parameters correctly
	EXPECT_FALSE(ParseString(" server { listen 80 8080; }"));
}

TEST_F(NginxStringConfigTest,NestedBracket){
	EXPECT_FALSE(ParseString(" server {{ listen ;}}"));

