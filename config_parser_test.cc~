#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, ExampleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, kbeezieConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("kbeezieEx", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, nginxWebConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("nginxWebEx", &out_config);

  EXPECT_TRUE(success);
}
