#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

//unit test for ToString of NginxConfigStatement
//foo bar
TEST(NginxConfigTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ(statement.ToString(0), "foo bar;\n"); //0 is the indentation level
}

TEST(NginxConfigParserTest, AnotherSimpleConfig) {
  std::string config_string = "foo bar;";
  std::stringstream config_stream(config_string);
  NginxConfigParser parser;
  NginxConfig config;
  EXPECT_TRUE(parser.Parse(&config_stream, &config));
  EXPECT_EQ(1, config.statements_.size())
    << "Config has one statments";
  EXPECT_EQ("foo", config.statements_.at(0)->tokens_[0]);
}


TEST(NginxConfigParserTest, InvalidConfig) {
  std::string config_string = "foo bar";
  std::stringstream config_stream(config_string);
  NginxConfigParser parser;
  NginxConfig config;
  EXPECT_FALSE(parser.Parse(&config_stream, &config));
}

//fixture??
class NginxStringConfigTest : public ::testing::Test {
protected:
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

//now we can remove all &config
TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size())
    << "Config has one statments";
  EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}


TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}


TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server { listen 80; }"));
}

TEST_F(NginxStringConfigTest, UnbalancedConfig) {
  EXPECT_FALSE(ParseString("server { listen 80;"));
}



