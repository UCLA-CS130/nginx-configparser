#include "gtest/gtest.h"
#include "config_parser.h"


// Test Fixture: used to parse config statements passed in as strings
// I.E. "foo bar;"
class NginxStringConfigTest : public ::testing::Test {

    protected:
        bool ParseString(const std::string config_string) {
            std::stringstream config_stream(config_string);
            return parser_.Parse(&config_stream, &out_config_); 
        }
        
        std::string GetToken(int statement_idx, int token_idx) {
            if (statement_idx < 0 || token_idx < 0)
                return NULL; 

            return out_config_.statements_[statement_idx]->tokens_[token_idx];    
        }

        // Limited to a child statement depth of 1
        std::string GetChildStatementToken(int statement_idx, int child_statement_idx, int token_idx) {
            if (statement_idx < 0 || token_idx < 0 || child_statement_idx < 0)
                return NULL;
            return out_config_.statements_[statement_idx]->child_block_->statements_[child_statement_idx]->tokens_[token_idx];
        }

        NginxConfigParser parser_; 
        NginxConfig out_config_; 
}; 

TEST(FileParsingTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success) << "Valid config files should be parsed successfully";
  EXPECT_EQ(2, out_config.statements_.size());
  EXPECT_EQ(3, out_config.statements_[1]->child_block_->statements_.size());
}

TEST_F(NginxStringConfigTest, ValidSimpleStatement) {
    EXPECT_TRUE(ParseString("foo bar;")); 
    EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statement"; 
    EXPECT_EQ("foo", GetToken(0,0)); 
}

TEST_F(NginxStringConfigTest, ValidStatementWhitespace) {
    EXPECT_TRUE(ParseString("foo             bar;"))  << "Config statements with additional white space should be parsed successfully";
    EXPECT_EQ(1, out_config_.statements_.size()); 
    EXPECT_EQ("foo", GetToken(0,0));
    EXPECT_EQ("bar", GetToken(0,1)); 
}

TEST_F(NginxStringConfigTest, ValidStatementTab) {
    EXPECT_TRUE(ParseString("foo	bar;")) << "Config statements with tabbing should be parsed successfully";
    EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statement"; 
    EXPECT_EQ("foo", GetToken(0,0));
    EXPECT_EQ("bar", GetToken(0,1)); 
}

TEST_F(NginxStringConfigTest, ValidStatementNested) {
    ASSERT_TRUE(ParseString("foo bar; server { fizz buzz; }")); 
    EXPECT_EQ(2, out_config_.statements_.size()) << "Config has 2 statements"; 
    EXPECT_EQ("foo", GetToken(0,0));
    EXPECT_EQ("bar", GetToken(0,1));
    EXPECT_EQ("fizz", GetChildStatementToken(1,0,0));
    EXPECT_EQ("buzz", GetChildStatementToken(1,0,1));
}

TEST_F(NginxStringConfigTest, ValidEmptyConfig) {
    ASSERT_TRUE(ParseString("")) << "Empty config files should still be parseable";
}

TEST_F(NginxStringConfigTest, ValidMultipleNestedConfigs) {
    ASSERT_TRUE(ParseString("foo bar; server { foo bar; http {foo bar;} }")) << "Config statements with multiple child blocks should be parsed successfully";
}

TEST_F(NginxStringConfigTest, ValidEmptyBlockStatement) {
    ASSERT_TRUE(ParseString("foo bar; server {}")) << "Empty config blocks should be parsed successfully";
}

TEST_F(NginxStringConfigTest, InvalidSimpleStatement) {
    ASSERT_FALSE(ParseString("foo bar")); 
}

TEST_F(NginxStringConfigTest, InvalidStatementMultipleSemiColon) {
    ASSERT_FALSE(ParseString("foo bar;;")); 
}

TEST_F(NginxStringConfigTest, InvalidStatementUnbalancedBraces_1) {
    ASSERT_FALSE(ParseString("foo bar; foo { foo bar; server { fizz buzz; }")) << "Curly braces must be balanced"; 
}

TEST_F(NginxStringConfigTest, InvalidStatementUnbalancedBraces_2) {
    ASSERT_FALSE(ParseString("foo bar; foo { foo bar; server { fizz buzz; { {")) << "Curly braces must be balanced";
}



