#include "gtest/gtest.h"
#include "config_parser.h"
#include <string>

// Tests basic config
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

// Tests ToString method on basic config
TEST(NginxConfigParserTest, ToString) {
    NginxConfigStatement statement;
    statement.tokens_.push_back("foo");
    statement.tokens_.push_back("bar");
    //the 0 controls the indentation level
    EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

// Tests ToString method that contains a block
TEST(NginxConfigParserTest, ToStringBlock) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("server");
	statement.tokens_.push_back("{");
	statement.tokens_.push_back("listen");
	statement.tokens_.push_back("80");
	statement.tokens_.push_back(";");
	statement.tokens_.push_back("}");
	EXPECT_EQ(statement.ToString(0), "server { listen 80 ; };\n");
}

class NginxStringConfigTest : public ::testing::Test {
    protected:
        bool ParseString(const std::string config_string) {
            std::stringstream config_stream(config_string);
            return parser_.Parse(&config_stream, &out_config_);
        }
 
        NginxConfigParser parser_;
        NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, ValidConfig) {
    EXPECT_TRUE(ParseString("foo bar;"));
    EXPECT_EQ(1, out_config_.statements_.size());
    EXPECT_EQ(out_config_.statements_.at(0)->ToString(0), "foo bar;\n");
}

TEST_F(NginxStringConfigTest, ValidConfigBlock) {
    EXPECT_TRUE(ParseString("server { listen 80; }"));
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
    EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, InvalidConfigBlock) {
    EXPECT_FALSE(ParseString("server listen 80; }"));
    EXPECT_FALSE(ParseString("server { listen 80;"));
}

TEST_F(NginxStringConfigTest, ValidConfigNested) {
	EXPECT_TRUE(ParseString("server { listen {foo 80;}}"));
}