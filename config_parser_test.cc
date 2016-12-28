#include "gtest/gtest.h"
#include "config_parser.h"

/*TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}
*/

  /* some shit we talked about in class */
  /* EXPECT_EQ is more permissive than ASSERT */
TEST(NginxConfigTest, ToStringSimple) {  
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ("foo bar;\n", statement.ToString(0)) << "foo bar wasn't serialized correctly";
}

class NginxStringConfigTest : public ::testing::Test {
	protected:
		bool ParseString(const std::string& config_string) {
			std::stringstream config_stream(config_string);
			return parser_.Parse(&config_stream, &config_);
		}
		NginxConfigParser parser_;
		NginxConfig config_;
};


TEST_F(NginxStringConfigTest, SimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse successfully.";
	EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement.";
	EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, BadConfig) {
	EXPECT_FALSE(ParseString("foo bar")) 
	<< "Bad config was parsed successfully.";

}

TEST_F(NginxStringConfigTest, EmptyConfig) {
	EXPECT_FALSE(ParseString(" ")) 
	<< "Bad config was parsed successfully.";

}

TEST_F(NginxStringConfigTest, NestedConfig) {
	EXPECT_TRUE(ParseString("server { listen 80; }")) 
	<< "Nested config parsed incorrectly.";
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
	EXPECT_TRUE(ParseString("foo { bar { baz auz; } }"))
	<< "Nested config parsed incorrectly.";
}

TEST_F(NginxStringConfigTest, CommentConfig) {
	EXPECT_TRUE(ParseString("# just fuck me up tbh."))
	<< "Comment config parsed incorrectly.";
}

TEST_F(NginxStringConfigTest, QuoteConfig) {
	EXPECT_TRUE(ParseString("printf('this is a comment');"))
	<< "Quote config did not parse correctly.";
} // how to test double quotes?

TEST_F(NginxStringConfigTest, DoubleQuoteConfig) {
	EXPECT_TRUE(ParseString("printf(\"this is a comment?\");"))
	<< "Double quote config did not parse correctly.";
}






/*
TEST_F(NginxStringConfigTest, SimpleConfig) {
	std::stringstream config_stream("foo bar;");
	NginxConfigParser parser;
	NginxConfig output;
	EXPECT_TRUE(parser.Parse(&config_stream, &output)) << "Didn't parser successfully"l
	EXPECT_EQ(1, output.statements_.size()) << "Didn't get one statement.";
	EXPECT_EQ("foo", output.statements_.at(0)->tokens_.at(0));
}

TEST(NginxConfigTest, EmptyConfig) {
	std::stringstream config_stream(" ");
	NginxConfigParser parser;
	NginxConfig output;
	EXPECT_FALSE(parser.Parse(&config_stream, &output)) << "Got empty string.";

}
*/