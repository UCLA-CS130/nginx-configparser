#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxStringParserTest : public ::testing::Test {
protected:

	bool parseString(const std::string& str) {
		std::stringstream ss(str);
		return parser.Parse(&ss, &out_config);
	}
	NginxConfigParser parser;
	NginxConfig out_config;
};

// Simple statements
TEST_F(NginxStringParserTest, ParseStatements) {
	EXPECT_TRUE(parseString("http1.1;"));
	EXPECT_TRUE(parseString("version 1.1;"));
	EXPECT_TRUE(parseString("version 1.1;\nrevision 4;\n"));
}

// Are empty {} allowed?
TEST_F(NginxStringParserTest, ParseContextEmpty) {
	EXPECT_TRUE(parseString("Events {\n}"));
}

// Sanity check {}
TEST_F(NginxStringParserTest, ParseContextSimple) {
	EXPECT_TRUE(parseString("html { port 4000; }"));
	EXPECT_TRUE(parseString("location /images/ { root /data/; }"));
}

// Multiple nesting
TEST_F(NginxStringParserTest, ParseContextDeep) {
	EXPECT_TRUE(parseString("server { location { root /data; }}"));
}

// Unbalanced curly braces
TEST_F(NginxStringParserTest, ParseUnbalanced) {
	EXPECT_FALSE(parseString("server { hello; }}"));
	EXPECT_FALSE(parseString("server { hello;"));
}

//// Just C++ things... abusing the unique_ptr
//TEST(NginxConfigParserTest, StatementCopyTest) {
//	ASSERT_TRUE(parseString("simple;"));
//	NginxConfigStatement stmt = 
//}