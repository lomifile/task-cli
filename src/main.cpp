#include <exception>
#include <iostream>
#include <string>

#include "Token.h"

int main() {
  Tokenizer *tokenizer = new Tokenizer("./sample-db.json");

  while (tokenizer->has_more_tokens()) {
    try {
      auto token = tokenizer->get_token();
      std::cout << static_cast<char>(token.type) << " " << token.value
                << std::endl;
    } catch (const std::exception &e) {
      break;
    }
  }

  tokenizer->close_file();
  return 0;
}
