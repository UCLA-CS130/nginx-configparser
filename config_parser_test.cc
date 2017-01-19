#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigTest, ToString){
  NginxConfigStatement statement;
  statement.tokens_.push_back("Statement with spaces");
  statement.tokens_.push_back("1");
  
  EXPECT_EQ(statement.ToString(0), "Statement with spaces 1;\n");
}

// Fixture to be used for testing the NginxConfigParser parse functions
class NginxConfigParserTest : public :: testing::Test {
  protected:
    NginxConfigParser parser_;
    NginxConfig output_config_;
};

// Basic Config
TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser_.Parse("example_config", &output_config_);
  
  EXPECT_TRUE(success);
}

// No File Config
TEST_F(NginxConfigParserTest, NotFoundConfig){
  bool failure = parser_.Parse("no_config_here", &output_config_);

  EXPECT_FALSE(failure);
}

// Nested Configuration
TEST_F(NginxConfigParserTest, NestedConfig){
  std::string config_string = 
  "server {"
  "  location {"
  "    /home/;"
  "  }"
  "}";

  std::stringstream config_stream(config_string);
  bool success = parser_.Parse(&config_stream, &output_config_);

  EXPECT_TRUE(success);
}

// Bad Brackets
TEST_F(NginxConfigParserTest, IncorrectBracketConfig){
  std::string bad_config = 
  "server {"
  "  /home/;"
  "}}"; 
  
  std::stringstream config_stream(bad_config);
  bool failure = parser_.Parse(&config_stream, &output_config_);  
  EXPECT_FALSE(failure);
}

TEST_F(NginxConfigParserTest, MissingEndStatement){
  std::string invalid_config = 
  "server {"
  "  /home/"
  "}";
  
  std::stringstream config_stream(invalid_config);
  bool failure = parser_.Parse(&config_stream, &output_config_);
  EXPECT_FALSE(failure);
}

TEST_F(NginxConfigParserTest, CheckOutputEq){
  std::string small_config =
  "server {"
  "  location {"
  "    /dev/null;"
  "  }"
  "}"
  "port 8080;";

  std::stringstream config_stream(small_config);
  bool mustBeTrue = parser_.Parse(&config_stream, &output_config_);
  ASSERT_TRUE(mustBeTrue);

  std::string result = output_config_.ToString();
  EXPECT_EQ(result, "server {\n  location {\n    /dev/null;\n  }\n}\nport 8080;\n");
} 
