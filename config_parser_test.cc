#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserTest : public ::testing::Test
{
protected:
    bool testSuccess(const char* file_name) {
        return parser.Parse(file_name, &out_config);
    }
private:
    NginxConfigParser parser;
    NginxConfig out_config; 
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = testSuccess("example_config");
  // Example program should be valid
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, SimpleConfigFakeFile) {
  bool success = testSuccess("this_is_not_a_file");
  // Shouldn't work on fake files
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CommentConfig) {
  bool success = testSuccess("example_config2");
  // Comments shouldn't cause errors
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, BracketConfig) {
  bool success = testSuccess("example_config3");
  // Should get an error if { is before anything else
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, SemicolonConfig) {
  bool success = testSuccess("example_config4");
  // Shouldn't have ; at start of program
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, BadCloseBracketsConfig) {
  bool success = testSuccess("example_config5");
  // Need ; before }
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, StrangeSemicolonConfig) {
  bool success = testSuccess("example_config6");
  /**
   * foo
   * ;
   * While strange, this is an acceptable file
   */
  EXPECT_TRUE(success);
}

// NOTE: Error was corrected in config_parser.cc
TEST_F(NginxConfigParserTest, StrangeSemicolon2Config) {
  bool success = testSuccess("example_config7");
  /**
   * foo;;
   * While strange, this is also acceptable (in c++, at least)
   */
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, EmptyConfig) {
  bool success = testSuccess("example_config8");
  // Empty files probably shouldn't be accepted
  EXPECT_FALSE(success);
}

// NOTE: Error was corrected in config_parser.cc
TEST_F(NginxConfigParserTest, NestedLoopConfig) {
  bool success = testSuccess("example_config9");
  // Nested loops should be accepted
  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToString) {
    NginxConfigStatement statement;
    // Empty string
    EXPECT_EQ(";\n", statement.ToString(0));

    statement.tokens_.push_back("foo");

    EXPECT_EQ("foo;\n", statement.ToString(0));

    statement.tokens_.push_back("bar");

    // String and indented string
    EXPECT_EQ("foo bar;\n", statement.ToString(0));
    EXPECT_EQ("  foo bar;\n", statement.ToString(1));
}