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

TEST(NginxConfigStatementTest, ToString) {
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
    };

    NginxConfigParser parser_;
    NginxConfig config_;
};

TEST_F(NginxStringConfigTest, SimpleTextConfig) {
    EXPECT_TRUE(ParseString("foo bar;"));
    // EXPECT_EQ("foo", config_.statements_[0]);
}


TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
    EXPECT_FALSE("foo bar");
}


TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(
        ParseString(
            "http {"
            "  foo bar;"
            "  server {"
            "    listen 80;"
            "  }"
            "}"
        )
    );
}

TEST(NginxConfigParserTest, FullConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("full_config_sample", &out_config);

  EXPECT_TRUE(success);
}
