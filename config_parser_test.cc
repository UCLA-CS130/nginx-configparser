#include "gtest/gtest.h"
#include "config_parser.h"
// Fixture to be used for testing the NginxConfigParser parse functions
class NginxConfigParserTest : public :: testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};

//Some example conf file
TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool example = parser.Parse("tests/example_config", &out_config);
  
  EXPECT_TRUE(example);
}

//File not existent
TEST_F(NginxConfigParserTest, NotFoundConfig){
  bool failure = parser.Parse("fake", &out_config);

  EXPECT_FALSE(failure);
}

//Uneven brace test cases
TEST_F(NginxConfigParserTest, twoEndingBracketsInRow) {
  bool twoEndBracketsInRow = parser.Parse("tests/ex3", &out_config);
    
  EXPECT_FALSE(twoEndBracketsInRow);
}

TEST_F(NginxConfigParserTest, missingCloseBracket) {
  bool missingClosingBracket = parser.Parse("tests/ex4", &out_config);
  
  EXPECT_FALSE(missingClosingBracket);
}

TEST_F(NginxConfigParserTest, missingStartBracket) {
  bool missingStartBracket = parser.Parse("tests/ex5", &out_config);
  
  EXPECT_FALSE(missingStartBracket);
}


//Check parse correctly
TEST_F(NginxConfigParserTest, CheckOutputEq){
  std::string toParse =
  "server {\n\t\"test\";\n}";

  std::stringstream config_stream(toParse);
  bool isTrue = parser.Parse(&config_stream, &out_config);
  ASSERT_TRUE(isTrue);

  std::string result = out_config.ToString();
  EXPECT_EQ(result, "server {\n  \"test\";\n}\n");
} 

TEST_F(NginxConfigParserTest, CheckOutputEq2){
  std::string toParse =
  "server {\n\t\"location ~ \.php$ {\";\n}";

  std::stringstream config_stream(toParse);
  bool isTrue = parser.Parse(&config_stream, &out_config);
  ASSERT_TRUE(isTrue);

  std::string result = out_config.ToString();
  EXPECT_EQ(result, "server {\n  \"location ~ .php$ {\";\n}\n");
} 
