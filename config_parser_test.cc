#include "gtest/gtest.h"
#include "config_parser.h"

class NginxStringConfigTest : public ::testing::Test {
 protected:
  bool ParseString(const std::string& config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  bool ParseFile(const std::string& config_filename) {
  	const char * filename = config_filename.c_str();;
  	return parser_.Parse(filename, &out_config_);
  }
  std::string ToStringSimple(std::vector<std::string> tokens_) {
    NginxConfigStatement statement;
    for (int i = 0; i < tokens_.size(); i++) {
      statement.tokens_.push_back(tokens_[i]);
    }
    return statement.ToString(0);
  }
  
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse successfully.";
  // We can also add expectations about the contents of the statements:
  EXPECT_EQ(1, out_config_.statements_.size()) << "Didn't get one statement";
  EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
	EXPECT_FALSE(ParseString("")) << "Got empty string wrong.";
}

TEST_F(NginxStringConfigTest, BadConfig) {
	EXPECT_FALSE(ParseString("foo bar")) 
	    << "Bad config was actually parsed successfully.";
  EXPECT_FALSE(ParseString("{}")) 
      << "Bad config was actually parsed successfully.";
}

TEST_F(NginxStringConfigTest, NestedParenConfig) {
	EXPECT_TRUE(ParseString("foo{foo{bar;}}")) 
	    << "Didn't parse nested Parentheses correctly.";
}

TEST_F(NginxStringConfigTest, ParseFile) {
	EXPECT_TRUE(ParseFile("example_config")) 
	    << "Didn't parse example_config correctly.";
}

TEST_F(NginxStringConfigTest, ParseFile2) {
	EXPECT_TRUE(ParseFile("example_config2")) 
	    << "Didn't parse example_config correctly.";
}

TEST_F(NginxStringConfigTest, ToStringSimple) {
  std::vector<std::string> tokens1;
  tokens1 = {"foo", "bar"};
  EXPECT_EQ("foo bar;\n", ToStringSimple(tokens1)) 
      << "foo bar wasn't serialized correctly";
  std::vector<std::string> tokens2;
  EXPECT_EQ(";\n", ToStringSimple(tokens2)) 
      << "empty string wasn't serialized correctly";
}
