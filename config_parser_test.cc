#include <iostream>
#include "config_parser.h"
#include "gtest/gtest.h"

// Test fixture to hold parser and parsed result
// Fresh fixture instances are created for every test case
class NginxConfigParserTest : public ::testing::Test {
protected:
    // No SetUp or TearDown required here
    NginxConfigParser parser;
    NginxConfig out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser.Parse("example_config", &out_config);
  std::cerr << out_config.ToString() << std::endl;

  ASSERT_TRUE(success);
}

TEST_F(NginxConfigParserTest, EmptyConfig) {
    bool failure = parser.Parse("", &out_config);

    ASSERT_FALSE(failure);
}
