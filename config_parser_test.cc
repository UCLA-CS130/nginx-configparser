#include "gtest/gtest.h"
#include "config_parser.h"

// Test that works
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

// Test that fails
TEST(NginxConfigParserTest2, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("braket_test", &out_config);

  EXPECT_TRUE(success);
}


