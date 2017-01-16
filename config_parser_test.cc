#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig1) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, MissingStartBlock) {
  NginxConfigParser parser;
  NginxConfig out_config;
  std::stringstream stream("server \n {listen 80;");

  bool success = parser.Parse(&stream, &out_config);

  EXPECT_FALSE(success);
}

TEST(NginxConfigParserTest, MissingEndBlock) {
  NginxConfigParser parser;
  NginxConfig out_config;
  std::stringstream stream("server \n listen 80; }");

  bool success = parser.Parse(&stream, &out_config);

  EXPECT_FALSE(success);
}

TEST(NginxConfigParserTest, SimpleConfig2) {
  NginxConfigParser parser;
  NginxConfig out_config;
  std::stringstream stream("server \n { listen {80;} \n server_name {foo.com;} \n root /home/ubuntu/sites/foo/;}");

  bool success = parser.Parse(&stream, &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, LastNestedBlockError) {
  NginxConfigParser parser;
  NginxConfig out_config;
  std::stringstream stream("server \n { listen {80;} \n server_name {foo.com;} \n root {/home/ubuntu/sites/foo/;}}");

  bool success = parser.Parse(&stream, &out_config);

  EXPECT_TRUE(success);
}