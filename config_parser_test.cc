#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "config_parser.h"


class NginxConfigParserTest : public ::testing::Test
{
protected:
  
  bool ParseString(const std::string& str)
  {
    out_config.statements_.clear();
    std::stringstream ss(str);
    return parser.Parse(&ss, &out_config);
  }
  
  NginxConfigParser parser;
  NginxConfig out_config;
};


TEST_F(NginxConfigParserTest, TokenTest) {
  EXPECT_TRUE(ParseString("foo;"));
  EXPECT_EQ("foo;\n", out_config.ToString());
  EXPECT_TRUE(ParseString("foo \"bar baz\"\t'bar baz'; # Comment"));
  EXPECT_EQ("foo \"bar baz\" 'bar baz';\n", out_config.ToString());
  EXPECT_FALSE(ParseString("foo"));
  EXPECT_FALSE(ParseString("foo bar"));
  EXPECT_FALSE(ParseString(""));
  EXPECT_FALSE(ParseString(" "));
  EXPECT_FALSE(ParseString("  "));
}


TEST_F(NginxConfigParserTest, ScopeTest) {
  EXPECT_TRUE(ParseString("foo \"bar\" { baz; }"));
  EXPECT_EQ("foo \"bar\" {\n  baz;\n}\n", out_config.ToString(0));
  EXPECT_TRUE(ParseString("foo { \"bar\" { baz; } }"));
  EXPECT_EQ("foo {\n  \"bar\" {\n    baz;\n  }\n}\n", out_config.ToString(0));
  EXPECT_FALSE(ParseString("foo \"bar\" baz; }"));
  EXPECT_FALSE(ParseString("foo \"bar\" { baz;"));
  EXPECT_FALSE(ParseString("foo { \"bar\" baz; } }"));
  EXPECT_FALSE(ParseString("foo \"bar\" { baz; } }"));
  EXPECT_FALSE(ParseString("foo \"bar\" baz; } }"));
  EXPECT_FALSE(ParseString("foo { \"bar\" { baz; }"));
  EXPECT_FALSE(ParseString("foo { \"bar\" { baz;"));
  EXPECT_FALSE(ParseString("{"));
  EXPECT_FALSE(ParseString("}"));
  EXPECT_FALSE(ParseString("{}"));
  EXPECT_FALSE(ParseString("{ }"));
  EXPECT_FALSE(ParseString("foo { }"));
}


TEST_F(NginxConfigParserTest, ExampleConfig) {
  EXPECT_TRUE(parser.Parse("example_config", &out_config));
  EXPECT_EQ("foo", out_config.statements_.at(0)->tokens_.at(0));
  EXPECT_FALSE(parser.Parse("null_config", &out_config));
}

