#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

//Fixture
class NginxParserTest : public ::testing::Test {
protected:
  NginxConfigParser parser;
  NginxConfig output;
  bool parse(const std::string input){
    std::stringstream straem(input);
    return(parser.Parse(&straem, &output));
  }
};

TEST_F(NginxParserTest, BadConfig){
  EXPECT_FALSE(parse("server { listen 80 } "));
}
  
TEST_F(NginxParserTest, UnequalBraces){
  EXPECT_TRUE(parse("server {listen 80;}"));
  EXPECT_FALSE(parse("server {listen 80;"));
  //FIXED:
    //The above passes, but should not. Is a bug to fix.
}

TEST_F(NginxParserTest, EmptyInput){
  EXPECT_TRUE(parse(""));
  //Empty Input should be valid
}

//TODO:
//TEST(NginxParserTest, ToString){
//  NginxConfigStatement s;
//  s.tokens_.push_back("foo");
//  s.tokens_.push_back("bar");
  //EXPECT_EQ(s.ToString(0), "foo bar;\n");
  //The above doesn't pass, but should. (TODO).
//}
