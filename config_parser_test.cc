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

TEST(NginxConfigStatementTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

class NginxStringConfigParserTest : public ::testing::Test {
protected:
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxStringConfigParserTest, SimpleTestConfig) {

	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, config_.statements_.size());

}

TEST(NginxConfigServerTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("server");
	EXPECT_EQ("server;\n", statement.ToString(0));
}

TEST(NginxConfigServerNameTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo.com");
	EXPECT_EQ("foo.com;\n", statement.ToString(0));
}
