#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigStatementTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

TEST(NginxConfigTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	statement.child_block_.reset(new NginxConfig);
	statement.child_block_.get()->statements_.emplace_back(new NginxConfigStatement);
	statement.child_block_.get()->statements_.back()->tokens_.push_back("nested");
	EXPECT_EQ(statement.ToString(0), "foo bar {\n  nested;\n}\n");
}

class NginxConfigParserTest : public ::testing::Test {
protected:
	bool ParseString(const std::string config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}
	bool parseFile(const char* file_name) {
		return parser_.Parse(file_name, &out_config_);
	}
	NginxConfigParser parser_;
	NginxConfig out_config_;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, out_config_.statements_.size());
	EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
}

TEST_F(NginxConfigParserTest, InvalidConfig) {
	EXPECT_FALSE(ParseString("foo bar"));
	EXPECT_FALSE(ParseString("half 'string"));
}

TEST_F(NginxConfigParserTest, NestedConfig) {
	EXPECT_TRUE(ParseString("server { listen 80; }"));
	EXPECT_EQ(1, out_config_.statements_.size());
	EXPECT_EQ(1, out_config_.statements_[0]->child_block_->statements_.size());
	EXPECT_EQ("listen", out_config_.statements_[0]->child_block_->statements_[0]->tokens_[0]);
}

TEST_F(NginxConfigParserTest, InvalidNestedConfig) {
	EXPECT_FALSE(ParseString("server { listen 80 }"));
	EXPECT_FALSE(ParseString("server { listen 80; "));
}

TEST_F(NginxConfigParserTest, ConfigFile) {
  EXPECT_TRUE(parseFile("example_config"));
}