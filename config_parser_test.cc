#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>

// As best I can tell, the point of this is to test with a file which is why
// we don't use TEST_N with NginxStringConfigTest. So I won't make the suggested
// change here.
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
    EXPECT_EQ(1, config_.statements_.size());
}

TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
    EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(ParseString("foo { "
                            "  bar { "
                            "    baz fasls;"
                            "  }"
                            "}"));
}
                
