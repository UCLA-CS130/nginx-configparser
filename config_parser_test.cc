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

class NginxFileConfigTest : public ::testing::Test {
protected:
  bool ParseFile(const char* file_name) {
    return parser_.Parse(file_name, &config_);
  }

  NginxConfigParser parser_;
  NginxConfig config_;
};

TEST_F(NginxFileConfigTest, SimpleConfigFromFile) {
  EXPECT_TRUE(ParseFile("test_files/example_config"));
  EXPECT_EQ(2, config_.statements_.size());
}

TEST_F(NginxFileConfigTest, BadConfigFromFile) {
  EXPECT_FALSE(ParseFile("test_files/bad_config"));
}

TEST_F(NginxFileConfigTest, NestedConfigFromFile) {
  EXPECT_TRUE(ParseFile("test_files/nested_config"));
}

TEST_F(NginxStringConfigTest, SimpleTextConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, config_.statements_.size());
}

TEST_F(NginxStringConfigTest, UnfinishedQuoteBadConfig) {
  EXPECT_FALSE(ParseString("server {\n listen \";\n}"));
}

TEST_F(NginxStringConfigTest, MissingSemicolonBadConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}

TEST(NginxConfigStatementTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ("foo bar;\n", statement.ToString(0));
}
