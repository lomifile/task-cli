#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace JSON {
class JSON_Node;
using JSON_Object = std::map<std::string, std::shared_ptr<JSON_Node>>;
using JSON_List = std::vector<std::shared_ptr<JSON_Node>>;

enum Type { OBJECT, LIST, STRING, NUMBER, BOOLEAN, NULL_TYPE };
union Values {
  JSON_Object *object;
  JSON_List *list;
  std::string *s;
  float f_value;
  bool b_value;
};

class JSON_Node {
private:
  Values values;

public:
  Type type;
  JSON_Node() {}
  JSON_Node(const Type &type) : type(type) {}

  auto get_object() {
    if (type == Type::OBJECT)
      return *this->values.object;
    throw std::logic_error("Improper return");
  }

  auto get_list() {
    if (type == Type::LIST)
      return *this->values.list;
    throw std::logic_error("Improper return");
  }

  auto get_string() {
    if (type == Type::STRING)
      return *this->values.s;
    throw std::logic_error("Improper return");
  }

  auto get_number() {
    if (type == Type::NUMBER)
      return this->values.f_value;
    throw std::logic_error("Improper return");
  }

  void set_object(JSON_Object *object) {
    this->values.object = object;
    type = Type::OBJECT;
  }

  void set_list(JSON_List *list) {
    this->values.list = list;
    type = Type::LIST;
  }

  void set_string(std::string *s) {
    this->values.s = s;
    type = Type::STRING;
  }

  void set_number(float f) {
    this->values.f_value = f;
    this->type = Type::NUMBER;
  }

  void print_node(int indentation);
  std::string to_string(int indentation);
};
} // namespace JSON
