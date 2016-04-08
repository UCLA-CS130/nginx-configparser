#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, EmptyFile) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("emptyfile_config", &out_config);

  // An empty file should not be able to be parsed.
  EXPECT_FALSE(success);
}

TEST(NginxConfigParserTest, InvalidFile) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("invalidfile_config", &out_config);

  // Parser should not be able to parse objects that are not files.
  EXPECT_FALSE(success);
}

TEST(NginxConfigParserTest, Comments) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("comment_config", &out_config);

  // Comments are allowed.
  EXPECT_TRUE(success);
}


TEST(NginxConfigParserTest, SemicolonStart) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("semistart_config", &out_config);

  // Files beginning with semicolons are not allowed.
  EXPECT_FALSE(success);
}

TEST(NginxConfigParserTest, SemicolonMultiple) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("semimult_config", &out_config);

  // Many semicolons in a row are allowed.
  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, NestedFor) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("nestedfor_config", &out_config);

  // Nested for loops should be allowed.
  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToStringSimple) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ("foo bar;\n", statement.ToString(0));
}
