#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>


TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);
  
  EXPECT_TRUE(success);
}

class NginxStringConfigTest : public :: testing::Test {
protected:
  bool ParseString(const std::string& config_string) {
       std::stringstream config_stream(config_string);
       return parser.Parse(&config_stream, &config);
     }
  NginxConfigParser parser;
  NginxConfig config;
};

/* unit tests
*/
TEST_F(NginxStringConfigTest, SimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, config.statements_.size());
}
