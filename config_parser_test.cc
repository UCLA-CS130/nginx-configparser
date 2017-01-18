#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, DocConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("nginx_doc_config", &out_config);

  EXPECT_TRUE(success);
}

//This test worked from the beginning and seems correct
//according to the example config on the nginx website
//although it does not conform with the grammar rules
//outlined in class. I did not fix anything related to
//this due to such cases existing in the full nginx
//example config.
TEST(NginxConfigParserTest, MissingSemicolonInConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config2", &out_config);

  EXPECT_TRUE(success);
}

//This test failed before the bugfix
TEST(NginxConfigParserTest, UnbalancedCurlyBracesInConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config3", &out_config);

  EXPECT_FALSE(success);
}

//This test failed before the bugfix
TEST(NginxConfigParserTest, DoubleCompositeStatementInConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config4", &out_config);

  EXPECT_TRUE(success);
}
