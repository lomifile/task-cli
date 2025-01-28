#pragma once
#include "Node.h"
#include "Token.h"
#include <memory>
#include <string>

class Parser {
private:
  Tokenizer _tokenizer;
  std::shared_ptr<JSON::JSON_Node> _root;
  std::shared_ptr<JSON::JSON_Node> _current;
  std::shared_ptr<JSON::JSON_Node> _parse_string();
  std::shared_ptr<JSON::JSON_Node> _parse_list();
  std::shared_ptr<JSON::JSON_Node> _parse_object();
  std::shared_ptr<JSON::JSON_Node> _parse_number();

public:
  Parser(const std::string &file_path) : _tokenizer(file_path) {};
  inline std::shared_ptr<JSON::JSON_Node> get_root() { return this->_root; }
  void parse();
};
