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

TEST(NginxConfigParserTest, ToString) {
    NginxConfigStatement statement;
    statement.tokens_.push_back("foo");
    statement.tokens_.push_back("bar");
    EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

class NginxStringConfigTest : public ::testing::Test { 
protected: 
    bool ParseString(const std::string& config_string) {
        std::stringstream config_stream(config_string);
        return parser_.Parse(&config_stream, &config_);
    }

    NginxConfigParser parser_;
    NginxConfig config_;
};

TEST_F(NginxStringConfigTest, SimpleTextConfig) {
    EXPECT_TRUE(ParseString("foo bar;"));
}

TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
    const std::string config_text = "foo bar";
    std::stringstream config_stream(config_text);
    NginxConfigParser parser;
    NginxConfig config; 
    EXPECT_FALSE(parser.Parse(&config_stream, &config));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(ParseString(
        "foo { "
        "   bar {"
        "      bax;"
        "   }"
        "}"));
}

TEST_F(NginxStringConfigTest, BadNestedConfig) {
    EXPECT_FALSE(ParseString(
        "foo { "
        "   bar {"
        "      bax;"
        "   }"
        "}"
        "}"
        "}"));
}


TEST_F(NginxStringConfigTest, BadNestedConfig2) {
    EXPECT_FALSE(ParseString(
        "{ test1"
        "    { this is a test} "
        "}}"));
}

TEST(NginxConfigParserTest, ToString2) {
    NginxConfigStatement statement;
    statement.tokens_.push_back("foo");
    statement.tokens_.push_back("{");
    statement.tokens_.push_back("bar");
    statement.tokens_.push_back("}");
    EXPECT_EQ("foo { bar };\n", statement.ToString(0));
}

TEST_F(NginxStringConfigTest, NestedConfig2) {
    EXPECT_TRUE(ParseString(
        "test1 { "
        "   test2 test3 { test4"
        "      test5;"
        "   }"
        "}"));
}

TEST_F(NginxStringConfigTest, NestedConfig3) {
    EXPECT_TRUE(ParseString(
        "test1 { "
        "   test2 {"
        "      test3 {"
        "         test4 {"
        "            test5 {"
        "               test6 {"
        "                  test7 {"
        "                     test8;"
        "                  }"
        "               }"
        "            }"
        "         }"
        "      }"
        "   }"
        "}"));
}
