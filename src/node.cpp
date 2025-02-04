#include "Node.h"
#include <iostream>
#include <ostream>
#include <string>

std::string JSON::JSON_Node::to_string(int indentation) {
  std::string space_string = std::string(indentation, ' ');
  std::string output_string = "";

  switch (type) {
  case JSON::Type::STRING:
    output_string += space_string + *values.s;
    output_string = '"' + output_string + '"';
    break;
  case JSON::Type::NUMBER:
    output_string += space_string + std::to_string(values.f_value);
    break;
  case JSON::Type::LIST: {
    output_string += "[\n";
    int index = 0;
    for (auto node : (*values.list)) {
      output_string += node->to_string(indentation + 1);
      if (index < (*values.list).size() - 1) {
        output_string += space_string + ", ";
      }
      index++;
    }
    output_string += space_string + "]\n";
    break;
  }
  case JSON::Type::OBJECT: {
    output_string += "{\n";
    for (auto i = (*values.object).begin(); i != (*values.object).end(); i++) {
      output_string += '"' + i->first + '"' + ":";
      output_string += i->second->to_string(indentation + 1);
      auto next = i;
      next++;
      if ((next) != (*values.object).end()) {
        output_string += space_string + ",";
      }
      output_string += space_string + "\n";
    }
    output_string += "}\n";
  }
  }

  return output_string;
}

void JSON::JSON_Node::print_node(int indentation) {
  std::cout << to_string(indentation);
}
