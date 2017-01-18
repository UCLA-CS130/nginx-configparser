#include "gtest/gtest.h"
#include "config_parser.h"

// first parameter is name of the test case, and second parameter is name of test within test case

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string config_s) {
		std::stringstream config_stream(config_s);

		return parser_.Parse(&config_stream, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxStringConfigTest, ParseStatement) {
	bool success = ParseString("port 1010;");

	EXPECT_TRUE(success);

	// statements size 1
	ASSERT_EQ(config_.statements_.size(), 1);

	// tokens size 2
	ASSERT_EQ(config_.statements_[0]->tokens_.size(),2);

	EXPECT_EQ(config_.statements_[0]->tokens_[0],"port");
	EXPECT_EQ(config_.statements_[0]->tokens_[1],"1010");
}

TEST_F(NginxStringConfigTest, NoSemicolon) {
	bool success = ParseString("port 1010");
	EXPECT_FALSE(success);
}

TEST_F(NginxStringConfigTest, EmptyString) {
	bool success = ParseString("");

	EXPECT_FALSE(success);
}

// Returns true still!!

TEST_F(NginxStringConfigTest, MissingOpenBrace) {
	bool success = ParseString("server  port 1010; }");

	EXPECT_FALSE(success);
}

TEST_F(NginxStringConfigTest, MissingClosingBrace) {
	bool success = ParseString("server { port 1010; ");

	EXPECT_FALSE(success);
}
