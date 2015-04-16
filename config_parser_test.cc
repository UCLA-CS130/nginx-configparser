#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>

class NginxStringConfigTest : public ::testing::Test {
    protected:
        bool ParseString(const std::string& config_string) {
            std::stringstream config_stream(config_string);
            return parser_.Parse(&config_stream, &config_);
        }

        NginxConfigParser parser_;
        NginxConfig config_;
};

TEST(NginxConfigParserTest, SimpleConfigFromFile) {
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

TEST(NginxConfigParserTest, SimpleTextConfig) {
   const std::string config_test = "foo bar;";
   std::stringstream config_stream(config_test);
   NginxConfigParser parser;
   NginxConfig config;

   EXPECT_TRUE(parser.Parse(&config_stream, &config));
}

TEST_F(NginxStringConfigTest, SimpleTextConfig) {
    EXPECT_TRUE(ParseString("foo bar;"));
}

TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
    EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, SimpleNestedConfig) {
    EXPECT_TRUE(ParseString("foo { foo bar; }"));
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
    EXPECT_TRUE(ParseString("foo { foo { foo bar; } }"));
}
