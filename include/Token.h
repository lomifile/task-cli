#include <fstream>

enum class Tokens : char {
  CURLY_OPEN = '{',
  CURLY_CLOSE = '}',
  COLON = ':',
  STRING = '"',
  ARRAY_OPEN = '[',
  ARRAY_CLOSE = ']',
  COMMA = ',',
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
};
