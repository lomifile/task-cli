#include <fstream>

enum Tokens {
  CURLY_OPEN,
  CURLY_CLOSE,
  COLON,
  STRING,
  ARRAY_OPEN,
  ARRAY_CLOSE,
  COMMA,
  NUMBER,
};

struct Token {
  std::string value;
  Tokens type;
  std::string to_stirng();
};

class Tokenizer {

public:
  Tokenizer(const std::string &file_path);
  char get_without_whitespace();
  Token get_token();
  int has_more_tokens();
  void rollback();
  void close_file();

private:
  std::fstream file;
  size_t prev_pos;
  void __parse_string(Token *token);
  void __parse_number(Token *token, char *c);
};
