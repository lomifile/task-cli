#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "Token.h"

// std::string Token::to_stirng() {}

Tokenizer::Tokenizer(const std::string &file_path) {
  this->file.open(file_path, std::ios::in);
  if (!this->file.good()) {
    std::cout << "File open error" << std::endl;
  }
}

char Tokenizer::get_without_whitespace() {
  char c = ' ';
  while ((c == ' ' || c == '\n')) {
    this->file.get(c);

    if ((c == ' ' || c == '\n') && !file.good()) {
      throw std::logic_error("Ran out of tokens");
    }
  }

  return c;
}

Token Tokenizer::get_token() {

  if (this->file.eof()) {
    std::cout << "Exhausted tokens" << std::endl;
  }

  this->prev_pos = file.tellg();

  char c = this->get_without_whitespace();

  struct Token token;

  switch (c) {
  case static_cast<char>(Tokens::STRING):
    token.type = Tokens::STRING;

    token.value = "";

    c = this->get_without_whitespace();

    while (c != '"') {
      token.value += c;
      c = this->get_without_whitespace();
    }

    std::cout << static_cast<char>(token.type) << " " << token.value
              << std::endl;

    break;

  case static_cast<char>(Tokens::ARRAY_OPEN):
    token.type = Tokens::ARRAY_OPEN;
    break;

  case static_cast<char>(Tokens::ARRAY_CLOSE):
    token.type = Tokens::ARRAY_CLOSE;
    break;

  case static_cast<char>(Tokens::CURLY_OPEN):
    token.type = Tokens::CURLY_OPEN;
    break;

  case static_cast<char>(Tokens::CURLY_CLOSE):
    token.type = Tokens::CURLY_CLOSE;
    break;
  case static_cast<char>(Tokens::COMMA):
    token.type = Tokens::COMMA;
    break;
  }

  return token;
}

void Tokenizer::close_file() { this->file.close(); }

int Tokenizer::has_more_tokens() { return !this->file.eof(); }
