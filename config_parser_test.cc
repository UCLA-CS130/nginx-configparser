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
