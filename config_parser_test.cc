#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxStringConfigTest : public :: testing :: Test {

  protected:
    bool ParseString(const std::string& config_string){

      std::stringstream config_stream(config_string);
      return parser_.Parse(&config_stream, &config_);

    }

    NginxConfigParser parser_;
    NginxConfig config_;


};

TEST_F(NginxStringConfigTest, SimpleConfig){
  EXPECT_TRUE(ParseString("foo bar;")) << "Didn't parse successfully";
  EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement";
  EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, LotsOfSemicolons){

  EXPECT_FALSE(ParseString(";;;;;;;;;;")) << "lotsa semicolons are ok?";

}

TEST_F(NginxStringConfigTest, WeirdBrackets){

  EXPECT_FALSE(ParseString("thing1 thing2{beep boop;};")) << "weird brackets allowed?";

}

TEST_F(NginxStringConfigTest, ExtraSpaces){

  EXPECT_TRUE(ParseString("a b      ;")) << "Did not allow extra spaces";

}

TEST_F(NginxStringConfigTest, ExtraReturnsAtTheEnd){

  EXPECT_TRUE(ParseString("blah;\n\n\n\n\n")) << "Did not allow extra returns at the end of config";

}

TEST_F(NginxStringConfigTest, TabsInConfig){

  EXPECT_TRUE(ParseString("hello	world;")) << "Did not allow tab in statement";

}

TEST_F(NginxStringConfigTest, SpacesInQuotes){

  EXPECT_TRUE(ParseString("drinks    \"banana shake\" \"orange juice\" \"red wine\";")) << "Did not allow spaces in quotes";

}

TEST_F(NginxStringConfigTest, BlockEndWithoutBlockStart){

  EXPECT_FALSE(ParseString("hello;}")) << "Allowed end of block without beginning of block";

}

TEST_F(NginxStringConfigTest, AdjacentBraces){ //Thanks to Jessica for this.

  EXPECT_TRUE(ParseString("mmm {bop {shoobydooby;}}")) << "Did not allow adjacent close braces.";
  EXPECT_FALSE(ParseString("mmm {{bop shoobydooby;}}")) << "Allowed adjacent open braces.";
}
