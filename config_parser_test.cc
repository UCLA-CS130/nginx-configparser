#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>

class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
private:
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, EventsConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("events_config", &out_config);

	EXPECT_TRUE(success);	
}

// Has nested elements
TEST(NginxConfigParserTest, WordpressConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;
	bool success = parser.Parse("wordpress_config", &out_config);

	EXPECT_TRUE(success);			
}

// Bare bones, with http, server, location, if statement
TEST(NginxConfigParserTest, BarebonesConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;
	bool success = parser.Parse("barebones_config", &out_config);

	EXPECT_TRUE(success);			
}

// Consecutive nested braces
TEST(NginxConfigParserTest, LocationConfig) {
	NginxConfigParser parser;
	NginxConfig out_config;
	bool success = parser.Parse("location_config", &out_config);

	EXPECT_TRUE(success);			
}

