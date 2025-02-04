#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "Token.h"

std::string Token::to_stirng() {
  switch (type) {
  case CURLY_OPEN:
    return "CURLY_OPEN";
  case CURLY_CLOSE:
    return "CURLY_CLOSE";
  case ARRAY_OPEN:
    return "ARRAY_OPEN";
  case ARRAY_CLOSE:
    return "ARRAY_CLOSE";
  case COMMA:
    return "COMMA";
  case COLON:
    return "COLON";
  case NUMBER:
    return "NUMBER: " + value;
  case STRING:
    return "STRING: " + value;
  }

  return "";
}

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

  if (c == '"')
    this->__parse_string(&token);
  else if (c == '-' || (c >= '0' && c <= '9'))
    this->__parse_number(&token, &c);
  else if (c == '[')
    token.type = Tokens::ARRAY_OPEN;
  else if (c == ']')
    token.type = Tokens::ARRAY_CLOSE;
  else if (c == '{')
    token.type = Tokens::CURLY_OPEN;
  else if (c == '}')
    token.type = Tokens::CURLY_CLOSE;
  else if (c == ':')
    token.type = Tokens::COLON;
  else if (c == ',')
    token.type = Tokens::COMMA;

  return token;
}

void Tokenizer::__parse_string(Token *token) {
  token->type = Tokens::STRING;
  token->value = "";
  char c = this->file.get();
  while (c != '"') {
    token->value += c;
    c = this->file.get();
  }
}

void Tokenizer::__parse_number(Token *token, char *c) {
  token->type = Tokens::NUMBER;
  token->value = "";
  token->value += *c;
  std::streampos prev_char_pos = this->file.tellg();
  while (*c == '-' || (*c >= '0' && *c <= '9') || *c == '.') {
    prev_char_pos = this->file.tellg();
    this->file.get(*c);

    if (this->file.eof())
      break;
    else {
      if (*c == '-' || (*c >= '0' && *c <= '9') || *c == '.') {
        token->value += *c;
      } else {
        file.seekg(prev_char_pos);
      }
    }
  }
}

void Tokenizer::rollback() {
  if (this->file.eof()) {
    file.clear();
  }
  file.seekg(this->prev_pos);
}

void Tokenizer::close_file() { this->file.close(); }

int Tokenizer::has_more_tokens() { return !this->file.eof(); }
