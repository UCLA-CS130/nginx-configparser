#include "gtest/gtest.h"
#include "config_parser.h"

/*
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}
*/

/*
TEST(NginxConfigParserTest, ToStringSimple) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ("foo bar;\n", statement.ToString(0)) 
	    << "foo bar wasn't serialized correctly";
}
*/


class NginxStringConfigTest : public testing::Test {
protected:
	bool ParseString(const std::string& config_string) {
        std::stringstream config_stream(config_string);
        return parser_.Parse(&config_stream, &config_);
	}
private:
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig) {
	EXPECT_TRUE(ParseString("foo{ha{bar;}}")) << "Didn't parse successfully.";
	// EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement";
	// EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
	EXPECT_FALSE(ParseString("")) << "Got empty string wrong.";
	EXPECT_FALSE(ParseString("foo bar")) 
	    << "Bad config was actually parsed successfully.";
};
/*
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
    EXPECT_EQ(1, config_.statements_.size());
    EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}
*/

/*
TEST(NginxConfigParserTest, SimpleConfig) {
	std::stringstream config_stream("foo bar;");
	NginxConfigParser parser;
	NginxConfig output;
	EXPECT_TRUE(parser.Parse(&config_stream, &output)) 
	    << "Didn't parse successfully.";
	EXPECT_EQ(1, output.statements_.size())
	    << "Didn't get one statement";
	EXPECT_EQ("foo", output.statements_.at(0)->tokens_.at(0));
}
*/
/*
TEST(NginxConfigParserTest, EmptyConfig) {
	std::stringstream config_stream("");
	NginxConfigParser parser;
	NginxConfig output;
	EXPECT_FALSE(parser.Parse(&config_stream, &output)) 
	    << "Got empty string wrong.";
}
*/

/*
TEST(NginxConfigParserTest, BadConfig) {
	std::stringstream config_stream("foo bar");
	NginxConfigParser parser;
	NginxConfig output;
	EXPECT_FALSE(parser.Parse(&config_stream, &output))
	    << "Bad config was actually parsed successfully.";
}
*/