#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

//Fixture implemented
class NginxStringConfigParserTest : public ::testing::Test{
protected:
bool ParseString (const std::string config_string){
  std:: stringstream config_stream(config_string);
  return parser_.Parse(&config_stream, &out_config_);
}
NginxConfigParser parser_;
NginxConfig out_config_;
};

TEST_F(NginxStringConfigParserTest, NotEmpty){
  EXPECT_FALSE(ParseString(""));
}

TEST_F(NginxStringConfigParserTest, MissingNestedParanthesis){
  EXPECT_FALSE(ParseString("fambli {fubli fibli;"));
}

TEST_F(NginxStringConfigParserTest, SpaceOkay){
  EXPECT_TRUE(ParseString("f        a      m      b l   ii i                        ;"));
}

TEST_F(NginxStringConfigParserTest, SemiOkay){
  EXPECT_TRUE(ParseString("flib;flab;"));
}

TEST_F(NginxStringConfigParserTest, AlphaOkay){
  EXPECT_TRUE(ParseString("ABCDEFGHIJKLMNOPQRSTUVWXYZ;"));
}
