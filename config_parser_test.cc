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
