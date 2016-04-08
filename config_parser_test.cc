#include "gtest/gtest.h"
#include "config_parser.h"

#include <sstream>
#include <string>

class NginxStringConfigTest : public ::testing::Test {
 protected:
  bool ParseString(const std::string& config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  // We can also add expectations about the contents of the statements:
  EXPECT_EQ(1, out_config_.statements_.size());
  EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
	EXPECT_FALSE(ParseString("")) << "Got empty string wrong";
}

TEST_F(NginxStringConfigTest, BracketConfig) {
	EXPECT_TRUE(ParseString("hello world;")) << "Got closed bracket wrong";
}

TEST(NginxConfigParserTest, ActiveCollabConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("activecollab_config", &out_config);
	EXPECT_TRUE(success);
}


TEST(NginxConfigParserTest, MoinMoinConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("activecollab_config", &out_config);
	EXPECT_TRUE(success);
}

