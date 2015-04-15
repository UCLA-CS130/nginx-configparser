#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
    NginxConfigParser parser;
    NginxConfig out_config;

    bool success = parser.Parse("example_config", &out_config);

    EXPECT_TRUE(success);
}


/*
 * proxy.config based on example from this website:
 * http://nginx.org/en/docs/beginners_guide.html#proxy
 */

TEST(NginxConfigParserTest, ProxyConfig) {
    NginxConfigParser parser;
    NginxConfig out_config;

    bool success = parser.Parse("proxy_config", &out_config);

    EXPECT_TRUE(success);
}
