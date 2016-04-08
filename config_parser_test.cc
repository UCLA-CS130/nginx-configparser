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

TEST(NginxConfigParserTest, ToStringSimple) {
    NginxConfigStatement statement;
    statement.tokens_.push_back("foo");
    statement.tokens_.push_back("bar");
    EXPECT_EQ("foo bar;\n", statement.ToString(0)) << 
        "foo bar wasn't serialized correctly";
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

TEST_F(NginxStringConfigTest, SimpleConfigStream) {

    EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse successfully";
    EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement";
    EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {

    EXPECT_FALSE(ParseString("")) << "Parsed empty config";
}

TEST_F(NginxStringConfigTest, BadConfig) {
    EXPECT_FALSE(ParseString("foo bar")) 
        << "Parsed config missing semicolon correctly";
}

TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(ParseString("server { listen 80; }"))
        << "Nested config parsed incorrectly";
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
    EXPECT_TRUE(ParseString("foo { bar { baz aux; } }"))
        << "Double nested config parsed incorrectly";
}

TEST_F(NginxStringConfigTest, MultiSpaceConfig) {
    EXPECT_TRUE(ParseString("foo         bar;"))
        << "Multiple spaces parsed incorrectly";
}

TEST_F(NginxStringConfigTest, MismatchedBracketsConfig) {
    EXPECT_FALSE(ParseString("foo { listen 80; }} "))
        << "Parsed unbalanced correctly";
}

TEST(NginxConfigParserTest, CommentedConfig) {
    NginxConfigParser parser;
    NginxConfig out_config;

    bool success = parser.Parse("commented_config", &out_config);

    EXPECT_TRUE(success);
}