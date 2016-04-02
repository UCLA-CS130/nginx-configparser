#include "gtest/gtest.h"
#include "config_parser.h"

class NginxStringConfigTest : public ::testing::Test {
protected:
	NginxConfigParser parser_;
	NginxConfig config_;
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
}; //Implemented Class to simplify stringstream tests.

TEST(NginxConfigParserTest, ExampleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success) << "example_config file did not parse successfully";
}

TEST(NginxConfigParserTest, MediaWikiConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config1", &out_config);

  EXPECT_TRUE(success) << "MediaWikiConfig file did not parse successfully";
}

TEST(NginxConfigParserTest, FastCGIConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config2", &out_config);

  EXPECT_TRUE(success) << "FastCGIConfig file did not parse successfully";
}


TEST_F(NginxStringConfigTest, CurlyBracesConfig) {

  bool success = ParseString("foo { bar; }");
  // EXPECT_EQ("expected", actual) << message
  // To STOP immediately, write an ASSERT
  EXPECT_TRUE(success) << "Error with curly braces";
}

TEST_F(NginxStringConfigTest, SimpleConfig){

	bool success1 = ParseString("foo bar;");

	EXPECT_TRUE(success1) << "one-line statement did not parse";
	//EXPECT_EQ(1, config_.statements_.size()) << "More than 1 statement";
	//EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) << "foo is not first statement";
}

//TEST(NginxConfigParserTest, MultiStatementConfig){
	//std::stringstream config_stream("foo bar");
	//NginxConfigParser parser1;
	//NginxConfig out_config1;
	//std::stringstream config_stream3("server { listen   80; server_name foo.com; root /home/ubuntu/sites/foo/; }");

	//bool success1 = parser1.Parse(&config_stream3, &out_config1);
	//EXPECT_TRUE(success1) << "Statement with 3 ; and curly braces did not parse";
	//EXPECT_EQ(1, config_.statements_.size()) << "More than 1 statement";
	//EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) << "foo is not first statement";
//}

TEST_F(NginxStringConfigTest, TripleBracesConfig){
	bool success1 = ParseString("foo { barbaz; true { fun; } more;}}");
	EXPECT_TRUE(success1) << "Statement triple curly braces did not parse";
	//EXPECT_EQ(1, config_.statements_.size()) << "More than 1 statement";
	//EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) << "foo is not first statement";
}
/* What do we test? 
	nested block of statements (curly braces) */
TEST(NginxConfigParserTest, ParserConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

 bool success = parser.Parse("config_parser_config", &out_config);

  EXPECT_TRUE(success) << "config_parser file did not parse successfully";
}