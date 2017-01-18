#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxStringParserTest : public ::testing::Test {
protected:

  bool parseString(const std::string& str) {
    std::stringstream ss(str);
    return parser.Parse(&ss, &out_config);
  }
  NginxConfigParser parser;
  NginxConfig out_config;
};

// Simple statements
TEST_F(NginxStringParserTest, ParseStatements) {
  EXPECT_TRUE(parseString("http1.1;"));
  EXPECT_TRUE(parseString("version 1.1;"));
  EXPECT_TRUE(parseString("version 1.1;\nrevision 4;\n"));
}

// Are empty {} allowed?
TEST_F(NginxStringParserTest, ParseContextEmpty) {
  EXPECT_TRUE(parseString("Events {\n}"));
}

// Sanity check {}
TEST_F(NginxStringParserTest, ParseContextSimple) {
  EXPECT_TRUE(parseString("html { port 4000; }"));
  EXPECT_TRUE(parseString("location /images/ { root /data/; }"));
  EXPECT_TRUE(parseString("html{port 1234;}"));
  EXPECT_TRUE(parseString("html\t\t\n{\t\nport 1234;\n\t}\n"));
}

// Multiple nesting
TEST_F(NginxStringParserTest, ParseContextDeep) {
  EXPECT_TRUE(parseString("server { location { root /data; }}"));
}

// Unbalanced curly braces
TEST_F(NginxStringParserTest, ParseUnbalanced) {
  EXPECT_FALSE(parseString("server { hello; }}"));
  EXPECT_FALSE(parseString("server { hello;"));
}

// Token test
TEST_F(NginxStringParserTest, ParseTokenCheck) {
  parseString("one two three four;");
  EXPECT_EQ(out_config.statements_[0]->tokens_, std::vector<std::string>({ "one", "two", "three", "four" }));
}

// Structure and Content test
TEST_F(NginxStringParserTest, ParseCheckStructure) {
  std::string str =
    "server {\n"
    "  zero {\n"
    "    0;\n"
    "    1;\n"
    "  }\n"
    "  one {\n"
    "    0;\n"
    "    1;\n"
    "  }\n"
    "  two {\n"
    "  }\n"
    "}\n";
  ASSERT_TRUE(parseString(str));

  // check
  ASSERT_EQ(str, out_config.ToString(0)); // printing
  // check root tokens
  ASSERT_TRUE(out_config.statements_[0]->tokens_ == std::vector<std::string>{ "server" });
  // check root children
  ASSERT_EQ(out_config.statements_.size(), 1); 


  // check children (zero, one, two)
  auto childptr = out_config.statements_[0]->child_block_.get();

  ASSERT_EQ(childptr->statements_.size(), 3);
  EXPECT_TRUE(childptr->statements_[0]->tokens_ == std::vector<std::string>{ "zero" });
  EXPECT_TRUE(childptr->statements_[1]->tokens_ == std::vector<std::string>{ "one" });
  EXPECT_TRUE(childptr->statements_[2]->tokens_ == std::vector<std::string>{ "two" });
  

  // children's children (0,1)
  EXPECT_EQ(childptr->statements_[0]->child_block_->statements_.size(), 2);
  EXPECT_EQ(childptr->statements_[1]->child_block_->statements_.size(), 2);
  EXPECT_EQ(childptr->statements_[2]->child_block_->statements_.size(), 0);

  // children's children's tokens
  EXPECT_TRUE(childptr->statements_[0]->child_block_->statements_[0]->tokens_ == std::vector<std::string>{ "0" });

  // side note - auto childptr = out_config.statements_[0]->child_block_; 
  //  is illegal since we can't have copies of a unique pointer -> we can't
  //  use the copy constructor or assignment op
  // a preference kind of thing?
}
