#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>

using namespace std;

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.emplace_back("foo");
  statement.tokens_.emplace_back("bar");
  EXPECT_EQ("chad smith;\n", statement.ToString(0));
}

class NginxStringConfigTest : public ::testing::Test {
protected:
  bool testParse(const string& input) {
    stringstream config_stream(input);
    return to_parse_.Parse(&config_stream, &configuration_);
  }

  NginxConfigParser to_parse_;
  NginxConfig configuration_;
};

TEST_F(NginxStringConfigTest, SimpleTextConfig) {
  EXPECT_TRUE(testParse("chad smith;"));
}

TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
  const string to_parse = "chad smith";
  stringstream config_stream(to_parse);
  NginxConfigParser parser;
  NginxConfig config; 
  EXPECT_FALSE(parser.Parse(&config_stream, &config));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(testParse(
        "foo { "
        "   bar {"
        "      bax;"
        "   }"
        "}"));
}

TEST_F(NginxStringConfigTest, BadNestedConfig) {
  EXPECT_FALSE(testParse(
        "foo { "
        "   bar {"
        "      bax;"
        "   }"
        "}}}}"));
}
