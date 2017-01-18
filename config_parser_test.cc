#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>

using namespace std;

// Initial test that was already given
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

// append two strings on back of statement and parse it
TEST(NginxConfigParserTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.emplace_back("chad");
  statement.tokens_.emplace_back("smith");
  EXPECT_EQ("chad smith;\n", statement.ToString(0));
}

// This class is here to make it so tests do not take
// so much code to write
class NginxStringConfigTest : public ::testing::Test {
protected:
  bool testParse(const string& input) {
    stringstream config_stream(input);
    return to_parse_.Parse(&config_stream, &configuration_);
  }

  NginxConfigParser to_parse_;
  NginxConfig configuration_;
};

// Simple test of parser on string "chad smith"
TEST_F(NginxStringConfigTest, SimpleTextConfig) {
  EXPECT_TRUE(testParse("chad smith;"));
}

// Test without use of class.  Does same as above test
TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
  const string to_parse = "chad smith";
  stringstream config_stream(to_parse);
  NginxConfigParser parser;
  NginxConfig config; 
  EXPECT_FALSE(parser.Parse(&config_stream, &config));
}

// test we expect to be true
TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(testParse(
        "foo { "
        "   bar {"
        "      bax;"
        "   }"
        "}"));
}

// test we expect to be false because
// we have too many '}' characters at the end
TEST_F(NginxStringConfigTest, BadNestedConfig) {
  EXPECT_FALSE(testParse(
        "foo { "
        "   bar {"
        "      bax;"
        "   }"
        "}}}}"));
}
