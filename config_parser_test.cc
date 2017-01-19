#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigTest, ToString)
{
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

TEST(NginxConfigParserTest, SimpleConfig)
{
  std::string config_string = "foo bar;";
  std::stringstream config_stream(config_string);
  NginxConfigParser parser;
  NginxConfig config;
  EXPECT_TRUE(parser.Parse(&config_stream, &config));
  EXPECT_EQ(1, config.statements_.size())
    << "Config has one statements";
    EXPECT_EQ("foo", config.statements_.at(0)->tokens_.at(0));
}

class NginxStringConfigTest : public :: testing :: Test {
protected:
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
  //NginxConfigStatement statements;
};

TEST_F(NginxStringConfigTest, AnotherSimpleConfig) 
{
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size())
    <<"Config has one statements";
   EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, InvalidConfig) 
{
  EXPECT_FALSE(ParseString("foo bar"));
}



