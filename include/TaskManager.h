#pragma once

#include "Node.h"
#include "Parser.h"
#include <memory>
#include <string>
#include <type_traits>

namespace Task {

class NodeFactory {
public:
  template <typename T>
  static inline std::shared_ptr<JSON::JSON_Node> create_node(const T *value) {
    std::shared_ptr<JSON::JSON_Node> node;
    if constexpr (std::is_same<T, float>::value) {
      node = std::make_shared<JSON::JSON_Node>();
      node->set_number((float)*value);
    } else if constexpr (std::is_same<T, std::string>::value) {
      node = std::make_shared<JSON::JSON_Node>();
      node->set_string((std::string *)value);
    } else if constexpr (std::is_same<T, JSON::JSON_List>::value) {
      std::shared_ptr<JSON::JSON_Node> node =
          std::make_shared<JSON::JSON_Node>();
      node->set_list((JSON::JSON_List *)value);
    } else if constexpr (std::is_same<T, JSON::JSON_Object>::value) {
      std::shared_ptr<JSON::JSON_Node> node =
          std::make_shared<JSON::JSON_Node>();
      node->set_object((JSON::JSON_Object *)value);
    }

    return node;
  }
};

class TaskManager {

private:
  JSON::JSON_Node *json;
  void append_to_file();
  float find_last_id();
  void flush_new_node(std::shared_ptr<JSON::JSON_Node> new_object_node);
  JSON::JSON_Node *find_node(const int &id);
  Parser *parser;

public:
  TaskManager() { this->parser = new Parser("./sample-db.json"); }
  TaskManager(Parser *parser) : parser(parser) {};
  ~TaskManager() = default;

  void create_new_task(std::string *task);
  void update_task(const int &id, std::string *task);
  void delete_task(const int &id);
  void print_tasks();
  JSON::JSON_Object *get_one(const int &id);
};
} // namespace Task
