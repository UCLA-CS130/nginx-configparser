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

TEST(NginxConfigParserTest, ExampleConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("example_config", &out_config);
	EXPECT_TRUE(success) << "Got simple example config wrong";
}

TEST(NginxConfigParserTest, MismatchedBracketConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("example_config2", &out_config);
	EXPECT_FALSE(success) << "Got extra open bracket wrong";
}

TEST(NginxConfigParserTest, MismatchedBracketConfig2) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("example_config3", &out_config);
	EXPECT_FALSE(success) << "Got extra closed bracket wrong";
}



TEST(NginxConfigParserTest, ActiveCollabConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("activecollab_config", &out_config);
	EXPECT_TRUE(success) << "Got example config ActiveCollab wrong";
}

TEST(NginxConfigParserTest, MoinMoinConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("MoinMoin", &out_config);
	EXPECT_TRUE(success) << "Got example config MoinMoin wrong";
}