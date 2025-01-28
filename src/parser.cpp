#include "Parser.h"
#include "Node.h"
#include <iostream>
#include <map>
#include <memory>
#include <ostream>

void Parser::parse() {
  while (this->_tokenizer.has_more_tokens()) {
    try {
      auto token = this->_tokenizer.get_token();

      switch (token.type) {
      case Tokens::STRING: {
        this->_tokenizer.rollback();
        std::shared_ptr<JSON::JSON_Node> node = this->_parse_string();
        if (!this->_root)
          this->_root = node;
        break;
      }

      case Tokens::CURLY_OPEN: {
        std::cout << "Object" << std::endl;
        std::shared_ptr<JSON::JSON_Node> parsed_object = this->_parse_object();
        if (!this->_root)
          this->_root = parsed_object;
        break;
      }

      case Tokens::ARRAY_OPEN: {
        std::shared_ptr<JSON::JSON_Node> parsed_list = this->_parse_list();
        if (!this->_root)
          this->_root = parsed_list;
        break;
      }

      case Tokens::NUMBER: {
        this->_tokenizer.rollback();
        std::shared_ptr<JSON::JSON_Node> parsed_number = this->_parse_number();
        if (!this->_root)
          this->_root = parsed_number;
        break;
      }
      }
    } catch (const std::exception &e) {
      break;
    }
  }
}

std::shared_ptr<JSON::JSON_Node> Parser::_parse_number() {
  std::shared_ptr<JSON::JSON_Node> node = std::make_shared<JSON::JSON_Node>();
  Token next_token = this->_tokenizer.get_token();
  float f_value = std::stof(next_token.value);
  node->set_number(f_value);
  return node;
}

std::shared_ptr<JSON::JSON_Node> Parser::_parse_list() {
  std::shared_ptr<JSON::JSON_Node> node = std::make_shared<JSON::JSON_Node>();
  JSON::JSON_List *list = new JSON::JSON_List();
  bool has_completed = false;

  while (!has_completed) {
    if (this->_tokenizer.has_more_tokens()) {
      Token next_token = this->_tokenizer.get_token();
      std::shared_ptr<JSON::JSON_Node> node;

      switch (next_token.type) {
      case Tokens::ARRAY_OPEN:
        node = this->_parse_list();
        break;
      case Tokens::CURLY_OPEN:
        node = this->_parse_object();
        break;
      case Tokens::STRING:
        this->_tokenizer.rollback();
        node = this->_parse_string();
        break;
      case Tokens::NUMBER:
        this->_tokenizer.rollback();
        node = this->_parse_number();
        break;
      }
      list->push_back(node);
      next_token = this->_tokenizer.get_token();
      if (next_token.type == Tokens::ARRAY_CLOSE) {
        has_completed = true;
      }
    }
  }

  node->set_list(list);
  return node;
}

std::shared_ptr<JSON::JSON_Node> Parser::_parse_object() {
  std::shared_ptr<JSON::JSON_Node> node = std::make_shared<JSON::JSON_Node>();
  JSON::JSON_Object *object = new JSON::JSON_Object();
  bool has_completed = false;

  while (!has_completed) {
    if (this->_tokenizer.has_more_tokens()) {
      Token next_token = this->_tokenizer.get_token();
      std::string key = next_token.value;
      std::shared_ptr<JSON::JSON_Node> node;
      this->_tokenizer.get_token();
      next_token = this->_tokenizer.get_token();

      switch (next_token.type) {
      case Tokens::ARRAY_OPEN:
        (*object)[key] = this->_parse_list();
        break;
      case Tokens::CURLY_OPEN:
        (*object)[key] = this->_parse_object();
        break;
      case Tokens::STRING:
        this->_tokenizer.rollback();
        (*object)[key] = this->_parse_string();
        break;
      case Tokens::NUMBER:
        this->_tokenizer.rollback();
        (*object)[key] = this->_parse_number();
        break;
      }

      next_token = this->_tokenizer.get_token();

      if (next_token.type == Tokens::CURLY_CLOSE) {
        has_completed = true;
        break;
      }
    }
  }

  node->set_object(object);
  return node;
}

std::shared_ptr<JSON::JSON_Node> Parser::_parse_string() {
  std::shared_ptr<JSON::JSON_Node> node = std::make_shared<JSON::JSON_Node>();
  Token token = this->_tokenizer.get_token();
  std::string *s_value = new std::string(token.value);
  node->set_string(s_value);
  return node;
}
