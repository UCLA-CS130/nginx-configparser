#include <string>
#include <sstream>
#include <iostream>
#include <memory>

#include "gtest/gtest.h"
#include "config_parser.h"


/* Test example file. */
TEST(NginxConfigParserTest, SimpleConfigFile) {
    NginxConfigParser parser;
    NginxConfig out_config;

    bool success = parser.Parse("example_config", &out_config);

    EXPECT_TRUE(success);
}


/* Class provides ability to create tests easily without repeating code. */
class NginxStringConfigTest : public ::testing::Test {
    protected:
    bool ParseString(const std::string& config_string) {
        std::stringstream config_stream(config_string);
        return parser_.Parse(&config_stream, &out_config_);
    }

    NginxConfigParser parser_;
    NginxConfig out_config_;
};


/* Tests one word. */
TEST_F(NginxStringConfigTest, SingleWordConfig) {
    EXPECT_TRUE(ParseString("foo;"));

    EXPECT_EQ(1, out_config_.statements_.size());
    EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

/* Tests multiple words. */
TEST_F(NginxStringConfigTest, MultipleWordConfig) {
    EXPECT_TRUE(ParseString("foo bar cool;"));

    EXPECT_EQ(1, out_config_.statements_.size());
    EXPECT_EQ(3, out_config_.statements_.at(0)->tokens_.size());

    EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
    EXPECT_EQ("bar", out_config_.statements_.at(0)->tokens_.at(1));
    EXPECT_EQ("cool", out_config_.statements_.at(0)->tokens_.at(2));
}

/* Test empty string. */
TEST_F(NginxStringConfigTest, EmptyConfig) {
    EXPECT_FALSE(ParseString(""));
    // We can also add expectations about the contents of the statements:
    EXPECT_EQ(0, out_config_.statements_.size());
}

/* Test quotes inside a string. */
TEST_F(NginxStringConfigTest, QuoteInStringConfig) {
    EXPECT_TRUE(ParseString("foo \"bar\";"));

    EXPECT_EQ(1, out_config_.statements_.size());
    EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
    EXPECT_EQ("\"bar\"", out_config_.statements_.at(0)->tokens_.at(1));
}

/* Test missing semicolon. */
TEST_F(NginxStringConfigTest, BadSemicolonConfig) {
    EXPECT_FALSE(ParseString("foo bar"));
}

/* Test multiple statements. */
TEST_F(NginxStringConfigTest, MultipleStatementsConfig) {
    EXPECT_TRUE(ParseString("foo bar; bar foo;"));

    EXPECT_EQ(2, out_config_.statements_.size());
    EXPECT_EQ(2, out_config_.statements_.at(0)->tokens_.size());
    EXPECT_EQ(2, out_config_.statements_.at(1)->tokens_.size());

    EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
    EXPECT_EQ("bar", out_config_.statements_.at(0)->tokens_.at(1));
    EXPECT_EQ("bar", out_config_.statements_.at(1)->tokens_.at(0));
    EXPECT_EQ("foo", out_config_.statements_.at(1)->tokens_.at(1));
}

/* Test single block. */
TEST_F(NginxStringConfigTest, NestedLoopConfig) {
    EXPECT_TRUE(ParseString("server { foo; }"));

    EXPECT_EQ(1, out_config_.statements_.size());
    EXPECT_EQ("server", out_config_.statements_.at(0)->tokens_.at(0));

    // std::shared_ptr<NginxConfigStatement> state = 
    //     out_config_.statements_.at(0)->child_block_->statements_.at(0);

    EXPECT_EQ(1, out_config_.statements_.at(0)->child_block_->statements_.at(0)->tokens_.size());
    EXPECT_EQ("foo", out_config_.statements_.at(0)->child_block_->statements_.at(0)->tokens_.at(0));
    // std::cout << out_config_.statements_.at(0)->child_block_->ToString(0) << std::endl;
    // std::cout << out_config_.statements_.at(0)->child_block_->statements_.at(0)->tokens_.at(0) << std::endl;
    // EXPECT_EQ("foo", out_config_.statements_.at(0)->child_b_.at(1));
}

/* Test multiple statements in block. */
TEST_F(NginxStringConfigTest, NestedLoopMultipleStatementsConfig) {
    EXPECT_TRUE(ParseString("server { foo; bar; }"));

    EXPECT_EQ(1, out_config_.statements_.size());
    EXPECT_EQ("server", out_config_.statements_.at(0)->tokens_.at(0));

    // NginxConfig *child_block = out_config_.statements_.at(0)->child_block_;
    EXPECT_EQ(2, out_config_.statements_.at(0)->child_block_->statements_.size());
    EXPECT_EQ(1, out_config_.statements_.at(0)->child_block_->statements_.at(0)->tokens_.size());
    EXPECT_EQ(1, out_config_.statements_.at(0)->child_block_->statements_.at(1)->tokens_.size());

    EXPECT_EQ("foo", out_config_.statements_.at(0)->child_block_->statements_.at(0)->tokens_.at(0));
    EXPECT_EQ("bar", out_config_.statements_.at(0)->child_block_->statements_.at(1)->tokens_.at(0));
}

// Test multiple entries in Nested Loop

// Test Comments

// Test new line

// Test DoubleNestedLoop