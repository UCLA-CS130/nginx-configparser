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


