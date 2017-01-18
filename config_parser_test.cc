#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

/* New test for FastCGIConfig */
TEST(NginxConfigParserTest, FastCGIConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("fastCGI_config", &out_config);

  EXPECT_TRUE(success);
}
