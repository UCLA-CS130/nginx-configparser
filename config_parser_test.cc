#include <fstream>
#include <stdio.h>
#include "gtest/gtest.h"
#include "config_parser.h"

/*****************************************************************
 * Testing Class: NginxConfigStatement
 * Number of Tests: 2
 *****************************************************************/
class NginxConfigStatementTest : public ::testing::Test {
protected:
  void pbParent(std::string token) {
    parentStatement_.tokens_.push_back(token);
  }
  void pbChild(std::string token) {
    childStatement_->tokens_.push_back(token);
  }
  void link() {
    config_->statements_.emplace_back(childStatement_);
    parentStatement_.child_block_.reset(config_);
  }
  NginxConfigStatement parentStatement_;
  NginxConfigStatement* childStatement_ = new NginxConfigStatement;
  NginxConfig* config_ = new NginxConfig;
};

TEST_F(NginxConfigStatementTest, ToStringNoChildBlock) {
  pbParent("foo");
  pbParent("bar");
  EXPECT_EQ("foo bar;\n", parentStatement_.ToString(0));
}

TEST_F(NginxConfigStatementTest, ToStringChildBlock) {
  pbParent("server");
  pbChild("port");
  pbChild("8000");
  link();
  EXPECT_EQ("server {\n  port 8000;\n}\n",
	    parentStatement_.ToString(0));
}

/*****************************************************************
 * Testing Class: NginxConfig
 * Number of Tests: 2
 *****************************************************************/
class NginxConfigTest : public ::testing::Test {
protected:
  void addStatement(std::vector<std::string> statement) {
    NginxConfigStatement* newState = new NginxConfigStatement;
    for (unsigned int i = 0; i < statement.size(); ++i) {
      newState->tokens_.push_back(statement[i]);
    }
    config_.statements_.emplace_back(newState);
  }
  NginxConfig config_;
};

TEST_F(NginxConfigTest, ToStringSingleStatement) {
  std::vector<std::string> s1 {"foo", "bar"};
  addStatement(s1);
  EXPECT_EQ("foo bar;\n", config_.ToString(0));
}

TEST_F(NginxConfigTest, ToStringMultipleStatements) {
  std::vector<std::string> s1 {"port", "8000"};
  std::vector<std::string> s2 {"server", "google.com"};
  std::vector<std::string> s3 {"root", "/home/"};
  addStatement(s1);
  addStatement(s2);
  addStatement(s3);
  EXPECT_EQ("port 8000;\nserver google.com;\nroot /home/;\n",
	    config_.ToString(0));
}

/*****************************************************************
 * Testing Class: NginxConfigParser
 * Number of Tests: 8
 *****************************************************************/
class NginxConfigParserTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    // Make a ghost config file for testing.
    config_file_.open(filename_);
    config_file_ << file_contents_;
    config_file_.close();
  }
  virtual void TearDown() {
    remove(filename_);
  }
  bool parseString(const std::string& config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &config_);
  }
  NginxConfigParser parser_;
  NginxConfig config_;
  std::ofstream config_file_;
  const char* filename_ = "NginxConfigParserTest_TestFile_0";
  std::string file_contents_ =
    "foo bar;\nserver {\n  port 8000;\n  hosts {\n    "
    "na google.com;\n    eu google.com/eu;\n  }\n}\n";
};

TEST_F(NginxConfigParserTest, ParseInvalidStringNoSemicolon) {
  std::string config_string =
    "foo bar\n";
  ASSERT_FALSE(parseString(config_string));
}

TEST_F(NginxConfigParserTest, ParseInvalidStringBadBlock) {
  std::string config_string =
    "server {\n port 8000;\n";
  ASSERT_FALSE(parseString(config_string));
}

TEST_F(NginxConfigParserTest, ParseInvalidStringNullString) {
  std::string config_string =
    "";
  ASSERT_FALSE(parseString(config_string));
}

TEST_F(NginxConfigParserTest, ParseSingleValidString) {
  std::string config_string =
    "foo bar;\n";
  ASSERT_TRUE(parseString(config_string));
  EXPECT_EQ(config_string, config_.ToString());
}

TEST_F(NginxConfigParserTest, ParseSingleBlock) {
  std::string config_string =
    "server {\n  port 8000;\n  host google.com;\n}\n";
  ASSERT_TRUE(parseString(config_string));
  EXPECT_EQ(config_string, config_.ToString());
}

TEST_F(NginxConfigParserTest, ParseNestedBlock) {
  std::string config_string =
    "server {\n  port 8000;\n  hosts {\n    na google.com;\n"
    "    eu google.com/eu;\n  }\n}\n";
  ASSERT_TRUE(parseString(config_string));
  EXPECT_EQ(config_string, config_.ToString());
}

TEST_F(NginxConfigParserTest, ParseNonFormatted) {
  std::string config_string_expected =
    "server {\n  port 8000;\n  hosts {\n    na google.com;\n"
    "    eu google.com/eu;\n  }\n}\n";
  std::string config_string_input =
    "server{\nport 8000;\nhosts{na google.com;\neu "
    "google.com/eu;}}";
  ASSERT_TRUE(parseString(config_string_input));
  EXPECT_EQ(config_string_expected, config_.ToString());
}

TEST_F(NginxConfigParserTest, ParseFile) {
  ASSERT_TRUE(parser_.Parse(filename_, &config_));
  EXPECT_EQ(file_contents_, config_.ToString());
}
