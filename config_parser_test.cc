#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, VhostsConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("vhosts_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigParserTest, ListenDefaultConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("listen_default_config", &out_config);

  EXPECT_TRUE(success);
}

// foo bar;
TEST(NginxConfigTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

class NginxStringConfigTest : public ::testing::Test {
  protected:
    bool ParseString(const std::string config_string) {
      std::stringstream config_stream(config_string);
      return parser_.Parse(&config_stream, &out_config_);
    }
    NginxConfigParser parser_;
    NginxConfig out_config_;
};

// Use TEST_F to use a fixture, helps with not repeating test setup
TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size())
    << "Config has one statements";
  EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server {\n\
                             listen 80;\n\
                             location ~ ^/forum/(?P.*)$ {\n\
                               return 301 $scheme://forum.domain.com/$1;\n\
                             }\n\
                           }"));
  // TODO: Test the contents of out_config_;
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
  EXPECT_TRUE(ParseString("user  www www;\n\
                             \n\
                             events {\n\
                               worker_connections 4096;\n\
                             }\n\
                             \n\
                             http {\n\
                               include conf/mime.types;\n\
                               server {\n\
                                 listen 80;\n\
                                 server_name domain1.com www.domain1.com;\n\
                                 \n\
                                 location ~ \.php$ {\n\
                                   fastcgi_pass 127.0.0.1:1025;\n\
                                 }\n\
                               }\n\
                            }"));
}

TEST_F(NginxStringConfigTest, UnbalancedConfig) {
  EXPECT_FALSE(ParseString("server { listen 80; "));
  EXPECT_FALSE(ParseString("server listen 80; }"));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
  EXPECT_FALSE(ParseString(""));
  EXPECT_FALSE(ParseString("# im the only one in here"));
  EXPECT_FALSE(ParseString("##\n"));
}

TEST_F(NginxStringConfigTest, OnlyCurlyConfig) {
  EXPECT_FALSE(ParseString("{}"));
}

TEST_F(NginxStringConfigTest, NoBlockNameConfig) {
  EXPECT_FALSE(ParseString("{ listen 80; }"));
}

TEST_F(NginxStringConfigTest, CommentConfig) {
  EXPECT_TRUE(ParseString("# hello world\n server { listen 80; }"));
  EXPECT_TRUE(ParseString("server {\n\
                            # hello world\n\
                            listen 80;\n\
                          }"));
  EXPECT_TRUE(ParseString("server { # simple reverse-proxy\n\
                             listen 80;\n\
                           }"));
}

TEST_F(NginxStringConfigTest, FullConfig) {
  EXPECT_TRUE(ParseString("user  www www;\n\
                           worker_processes 5;\n\
                           error_log logs/error.log;\n\
                           pid       logs/nginx.pid;\n\
                           \n\
                             events {\n\
                               worker_connections 4096;\n\
                             }\n\
                             \n\
                             http {\n\
                               include conf/mime.types;\n\
                               include /etc/nginx/proxy.conf;\n\
                               include /etc/nginx/fastcgi.conf;\n\
                               index   index.html index.htm index.php;\n\
                               \n\
                               default_type application/octet-stream;\n\
                               log_format   main '$remote_addr - $remote_user \
                                 [$time_local] $status \"$request\" $body_bytes_sent\
                                 \"$http_referer\" \"$http_user_agent\" \
                                 \"$http_x_forwarded_for\"';\n\
                               access_log   logs/access.log main;\n\
                               sendfile     on;\n\
                               tcp_nopush    on;\n\
                               server_names_hash_bucket_size 128; #this seems to be required for some vhosts\n\
                               \n\
                               server {\n\
                                 listen 80;\n\
                                 server_name domain1.com www.domain1.com;\n\
                                 access_log  logs/domain1.access.log main;\n\
                                 root        html;\n\
                                 \n\
                                 location ~ \.php$ {\n\
                                   fastcgi_pass 127.0.0.1:1025;\n\
                                 }\n\
                               }\n\
                               \n\
                               server { # simple reverse-proxy\n\
                                 listen     80;\n\
                                 server_name domain2.com www.domain2.com;\n\
                                 access_log  logs/domain2.access.log main;\n\
                                 \n\
                                 # serve static files\n\
                                 location ~ ^/(images|javascript|js|css|flash|media|static)/ {\n\
                                   root    /var/www/virtual/big.server.com/htdocs;\n\
                                   expires 30d;\n\
                                 }\n\
                                 \n\
                                 # pass requests for dynamic content to rails/turbogears/zope, et al\n\
                                 location / {\n\
                                   proxy_pass     http://127.0.0.1:8080;\n\
                                 }\n\
                               }\n\
                               \n\
                               upstream big_server_com {\n\
                                 server 127.0.0.3:8000 weight=5;\n\
                                 server 127.0.0.3:8001 weight=5;\n\
                                 server 192.168.0.1:8000;\n\
                                 server 192.168.0.1:8001;\n\
                               }\n\
                               \n\
                               server { #simple load balancing\n\
                                 listen       80;\n\
                                 server_name  big.server.com;\n\
                                 access_log   logs/big.server.access.log main;\n\
                                 \n\
                                 location / {\n\
                                   proxy_pass http://big_server_com;\n\
                                 }\n\
                               }\n\
                            }"));
}
