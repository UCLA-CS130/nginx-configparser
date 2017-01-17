#include "gtest/gtest.h"
#include "config_parser.h"



class NginxConfigParserStringTest : public ::testing::Test {
protected:
 	bool ParseString(const std::string config_string) {
 		std::stringstream config_stream(config_string);
 		return parser_.Parse(&config_stream, &out_config_);
 	}
 	NginxConfigParser parser_;
 	NginxConfig out_config_;
 };



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

TEST_F(NginxConfigParserStringTest, SimpleConfig1) {

	
	ASSERT_TRUE(ParseString("foo bar;"));
	ASSERT_EQ(1, out_config_.statements_.size());
	EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
  
	
}

TEST_F(NginxConfigParserStringTest, InvalidConfig) {

	EXPECT_FALSE(ParseString("foo bar"));
	
  
}

TEST_F(NginxConfigParserStringTest, MultipleStatements) {
	ASSERT_TRUE(ParseString("foo bar {foo bar; } foobar;"));
	// printf("adsfsdf%s\n", out_config_.ToString().c_str());
	EXPECT_EQ(2, out_config_.statements_.size());

}

TEST_F(NginxConfigParserStringTest, InnerStatement) {
	ASSERT_TRUE(ParseString("foo bar {foo barr; bar foo;} foobar;"));
	// printf("adsfsdf%s\n", out_config_.ToString().c_str());
	ASSERT_EQ(2, out_config_.statements_.size());
	EXPECT_EQ(2, out_config_.statements_[0].get()->child_block_->statements_.size());
	//printf("%s\n", out_config_.statements_[0].get()->child_block_->statements_[1]->ToString(0).c_str());

}

TEST_F(NginxConfigParserStringTest, CurlyConfig) {
	ASSERT_TRUE(ParseString("foo bar {foo bar; foo bar;}"));
	EXPECT_EQ(1, out_config_.statements_.size());

}

TEST_F(NginxConfigParserStringTest, UnbalancedCurlyConfig) {
	EXPECT_FALSE(ParseString("foo bar {foo bar; "));
	EXPECT_FALSE(ParseString("foo bar {foo bar {foo bar; }"));

}

TEST_F(NginxConfigParserStringTest, EmbedCurlyConfig) {
	ASSERT_TRUE(ParseString("foo bar { foo bar {fooo bar;} }"));
	ASSERT_EQ(1, out_config_.statements_.size());
	ASSERT_EQ(1, out_config_.statements_[0].get()->child_block_->statements_.size());
	EXPECT_EQ(1, out_config_.statements_[0].get()->child_block_->statements_[0].get()->child_block_->statements_.size());
	// printf("%s\n", out_config_.statements_[0].get()->child_block_->statements_[0]->ToString(0).c_str());
	printf("%s\n", out_config_.statements_[0].get()->child_block_->statements_[0].get()->child_block_->statements_[0]->ToString(0).c_str());


}