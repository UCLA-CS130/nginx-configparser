#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigTest, ToStringSimple) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ("foo bar;\n", statement.ToString(0));
}
