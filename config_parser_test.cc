#include "gtest/gtest.h"
#include "config_parser.h"

#include <sstream>
#include <string>

#include <stdio.h>

class NginxStringConfigTest : public ::testing::Test{
protected:
	bool ParseString(const std::string& config_string){
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
	bool ParseFile(const char* file_name){
		return parser_.Parse(file_name, &config_);
	}
	
	NginxConfigParser parser_;
	NginxConfig config_;

};

// Simple Test Cases
TEST_F(NginxStringConfigTest, Simple) {
	// Testing the Config "foo bar"
	EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse foo bar successfully";
  	EXPECT_EQ(1, config_.statements_.size()) 
  		<< "foo bar was not parsed as one statement";
  	EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) 
  		<< "foo was not first token, as expected";
}

TEST_F(NginxStringConfigTest, Empty){
	// Testing an Empty config
  	EXPECT_FALSE(ParseString("")) << "Empty should return false";
}

TEST_F(NginxStringConfigTest, MissingSemicolon){
  	// No Semi-colin
  	EXPECT_FALSE(ParseString("foo bar")) << "Missing semicolon should raise error";
}

TEST_F(NginxStringConfigTest, Nested){
  	// Test nested config
	EXPECT_TRUE(ParseString("server {listen 80;}")) 
		<< "Didn't parse nested config successfully";
	// Check the nested statement
	EXPECT_EQ(1, config_.statements_.size()) 
		<< "nested config was not parsed as one statement";
	EXPECT_EQ(2, config_.statements_.at(0)->child_block_->statements_.at(0)
		->tokens_.size())
		<< "There should be 2 tokens in the child block";
}

TEST_F(NginxStringConfigTest, DoubleNested){
  	// Test nested config
	EXPECT_TRUE(ParseString("foo {bar {baz qux; } }")) 
		<< "Didn't parse double nested config successfully";
	// Check 1st level of the nested statement
	EXPECT_EQ(1, config_.statements_.size()) 
		<< "nested config was not parsed as one statement";
	EXPECT_EQ(1, config_.statements_.at(0)->child_block_->statements_.at(0)
		->tokens_.size())
		<< "There should be 1 tokens in the child block";
	// Check 2nd level of the nested statement
	EXPECT_EQ(1, config_.statements_.at(0)->child_block_->statements_.at(0)
		->child_block_->statements_.size())
		<< "2nd nested config not parsed as one statement";
		
	EXPECT_EQ(2, config_.statements_.at(0)->child_block_->statements_.at(0)
		->child_block_->statements_.at(0)->tokens_.size())
		<< "There should be 2 tokens in the second nested block -- fuck you!!";
}

TEST_F(NginxStringConfigTest, MismatchBracket){
	EXPECT_FALSE(ParseString("server {listen 80;}}")) 
		<< "Didn't catch the mismatch error successfully";

}

TEST_F(NginxStringConfigTest, Comment){
	EXPECT_FALSE(ParseString("# This is a comment!!!!")) 
		<< "Didn't parse comment successfully";
  	EXPECT_EQ(0, config_.statements_.size()) 
  		<< "comment was incorrectly recognized as a statement";
}


// Testing NginxConfig's tosString also tests NginxConfigStatement's toString.
// Got the idea to do this test from my partner
TEST_F(NginxStringConfigTest, ToString){
	EXPECT_TRUE(ParseString("server {listen 80;}"));
  	EXPECT_EQ("server {\n  listen 80;\n}\n", config_.ToString()) 
  		<< "ToString function does not work";
}



// More complex test cases (Full configuration files)
TEST_F(NginxStringConfigTest, ComplexConfigs) {  
  	EXPECT_TRUE(ParseFile("example_config")) << "Didn't parse example_config successfully";
  	EXPECT_EQ(2, config_.statements_.size()) 
  		<< "wrong number of statements";
  	EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) 
  		<< "foo was not first token, as expected";
  	
  	EXPECT_TRUE(ParseFile("long_example_config")) 
  		<< "Didn't parse long_example_config successfully";
  	
}

