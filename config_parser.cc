// Usage: ./config_parser <path to config file>
//
// See:
//   http://wiki.nginx.org/Configuration
//   http://blog.martinfjordvald.com/2010/07/nginx-primer/
//
// How Nginx does it:
//   http://lxr.nginx.org/source/src/core/ngx_conf_file.c

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <stack>
#include <vector>

class NginxConfig;

class NginxConfigStatement {
 public:
  std::string ToString(int depth);
  std::vector<std::string> tokens_;
  std::unique_ptr<NginxConfig> child_block_;
};

class NginxConfig {
 public:
  std::string ToString(int depth = 0) {
    std::string serialized_config;
    for (const auto& statement : statements_) {
      serialized_config.append(statement->ToString(depth));
    }
    return serialized_config;
  }
  std::vector<std::shared_ptr<NginxConfigStatement>> statements_;
};

std::string NginxConfigStatement::ToString(int depth) {
  std::string serialized_statement;
  for (int i = 0; i < depth; ++i) {
    serialized_statement.append("  ");
  }
  for (unsigned int i = 0; i < tokens_.size(); ++i) {
    if (i != 0) {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }
  if (child_block_.get() != nullptr) {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->ToString(depth + 1));
    for (int i = 0; i < depth; ++i) {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  } else {
    serialized_statement.append(";");
  }
  serialized_statement.append("\n");
  return serialized_statement;
}

class NginxConfigParser {
 private:
  enum TokenType {
    TOKEN_TYPE_START = 0,
    TOKEN_TYPE_NORMAL = 1,
    TOKEN_TYPE_START_BLOCK = 2,
    TOKEN_TYPE_END_BLOCK = 3,
    TOKEN_TYPE_COMMENT = 4,
    TOKEN_TYPE_STATEMENT_END = 5,
    TOKEN_TYPE_EOF = 6,
    TOKEN_TYPE_ERROR = 7
  };

  const char* TokenTypeAsString(TokenType type) {
    switch (type) {
      case TOKEN_TYPE_START:         return "TOKEN_TYPE_START";
      case TOKEN_TYPE_NORMAL:        return "TOKEN_TYPE_NORMAL";
      case TOKEN_TYPE_START_BLOCK:   return "TOKEN_TYPE_START_BLOCK";
      case TOKEN_TYPE_END_BLOCK:     return "TOKEN_TYPE_END_BLOCK";
      case TOKEN_TYPE_COMMENT:       return "TOKEN_TYPE_COMMENT";
      case TOKEN_TYPE_STATEMENT_END: return "TOKEN_TYPE_STATEMENT_END";
      case TOKEN_TYPE_EOF:           return "TOKEN_TYPE_EOF";
      case TOKEN_TYPE_ERROR:         return "TOKEN_TYPE_ERROR";
      default:                       return "Unknown token type";
    }
  }

  enum TokenParserState {
    TOKEN_STATE_INITIAL_WHITESPACE = 0,
    TOKEN_STATE_SINGLE_QUOTE = 1,
    TOKEN_STATE_DOUBLE_QUOTE = 2,
    TOKEN_STATE_TOKEN_TYPE_COMMENT = 3,
    TOKEN_STATE_TOKEN_TYPE_NORMAL = 4
  };

  TokenType ParseToken(std::istream* input, std::string* value) {
    TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
    while (input->good()) {
      const char c = input->get();
      if (!input->good()) {
        break;
      }
      switch (state) {
        case TOKEN_STATE_INITIAL_WHITESPACE:
          switch (c) {
            case '{':
              *value = c;
              return TOKEN_TYPE_START_BLOCK;
            case '}':
              *value = c;
              return TOKEN_TYPE_END_BLOCK;
            case '#':
              *value = c;
              state = TOKEN_STATE_TOKEN_TYPE_COMMENT;
              continue;
            case '"':
              *value = c;
              state = TOKEN_STATE_DOUBLE_QUOTE;
              continue;
            case '\'':
              *value = c;
              state = TOKEN_STATE_SINGLE_QUOTE;
              continue;
            case ';':
              *value = c;
              return TOKEN_TYPE_STATEMENT_END;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
              continue;
            default:
              *value += c;
              state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
              continue;
          }
        case TOKEN_STATE_SINGLE_QUOTE:
          // TODO: the end of a quoted token should be followed by whitespace.
          // TODO: Maybe also define a QUOTED_STRING token type.
          // TODO: Allow for backslash-escaping within strings.
          *value += c;
          if (c == '\'') {
            return TOKEN_TYPE_NORMAL;
          }
          continue;
        case TOKEN_STATE_DOUBLE_QUOTE:
          *value += c;
          if (c == '"') {
            return TOKEN_TYPE_NORMAL;
          }
          continue;
        case TOKEN_STATE_TOKEN_TYPE_COMMENT:
          if (c == '\n' || c == '\r') {
            return TOKEN_TYPE_COMMENT;
          }
          *value += c;
          continue;
        case TOKEN_STATE_TOKEN_TYPE_NORMAL:
          if (c == ' ' || c == '\t' || c == '\n' || c == '\t' ||
              c == ';' || c == '{' || c == '}') {
            input->unget();
            return TOKEN_TYPE_NORMAL;
          }
          *value += c;
          continue;
      }
    }

    // If we get here, we reached the end of the file.
    if (state == TOKEN_STATE_SINGLE_QUOTE ||
        state == TOKEN_STATE_DOUBLE_QUOTE) {
      return TOKEN_TYPE_ERROR;
    }

    return TOKEN_TYPE_EOF;
  }

 public:
  NginxConfigParser() {}

  bool Parse(std::istream* config_file, NginxConfig* config) {
    std::stack<NginxConfig*> config_stack;
    config_stack.push(config);
    TokenType last_token_type = TOKEN_TYPE_START;
    TokenType token_type;
    while (true) {
      std::string token;
      token_type = ParseToken(config_file, &token);
      printf ("%s: %s\n", TokenTypeAsString(token_type), token.c_str());
      if (token_type == TOKEN_TYPE_ERROR) {
        break;
      }

      if (token_type == TOKEN_TYPE_COMMENT) {
        // Skip comments.
        continue;
      }

      if (token_type == TOKEN_TYPE_START) {
        // Error.
        break;
      } else if (token_type == TOKEN_TYPE_NORMAL) {
        if (last_token_type == TOKEN_TYPE_START ||
            last_token_type == TOKEN_TYPE_STATEMENT_END ||
            last_token_type == TOKEN_TYPE_START_BLOCK ||
            last_token_type == TOKEN_TYPE_END_BLOCK ||
            last_token_type == TOKEN_TYPE_NORMAL) {
          if (last_token_type != TOKEN_TYPE_NORMAL) {
            config_stack.top()->statements_.emplace_back(new NginxConfigStatement);
          }
          config_stack.top()->statements_.back().get()->tokens_.push_back(token);
        } else {
          // Error.
          break;
        }
      } else if (token_type == TOKEN_TYPE_STATEMENT_END) {
        if (last_token_type != TOKEN_TYPE_NORMAL) {
          // Error.
          break;
        }
      } else if (token_type == TOKEN_TYPE_START_BLOCK) {
        if (last_token_type != TOKEN_TYPE_NORMAL) {
          // Error.
          break;
        }
        NginxConfig* const new_config = new NginxConfig;
        config_stack.top()->statements_.back().get()->child_block_.reset(new_config);
        config_stack.push(new_config);
      } else if (token_type == TOKEN_TYPE_END_BLOCK) {
        if (last_token_type != TOKEN_TYPE_STATEMENT_END) {
          // Error.
          break;
        }
        config_stack.pop();
      } else if (token_type == TOKEN_TYPE_EOF) {
        if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
            last_token_type != TOKEN_TYPE_END_BLOCK) {
          // Error.
          break;
        }
        return true;
      } else {
        // Error. Unknown token.
        break;
      }
      last_token_type = token_type;
    }

    printf ("Bad transition from %s to %s\n",
            TokenTypeAsString(last_token_type),
            TokenTypeAsString(token_type));
    return false;
  }

  bool Parse(const char* file_name, NginxConfig* config) {
    std::ifstream config_file;
    config_file.open(file_name);
    if (!config_file.good()) {
      printf ("Failed to open config file: %s\n", file_name);
      return false;
    }

    const bool return_value =
      Parse(dynamic_cast<std::istream*>(&config_file), config);
    config_file.close();
    return return_value;
  }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./config_parser <path to config file>\n");
    return 1;
  }
  NginxConfigParser config_parser;
  NginxConfig config;
  config_parser.Parse(argv[1], &config);
  printf("%s", config.ToString().c_str());
  return 0;
}
