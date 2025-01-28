#include <cstring>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include "Node.h"
#include "TaskManager.h"

float Task::TaskManager::find_last_id() {
  auto list = this->parser->get_root()->get_list();

  float largest = 0;
  for (auto itr = list.begin(); itr != list.end(); itr++) {
    auto object = itr->get()->get_object();
    auto id = object["id"];

    if (id->get_number() > largest)
      largest = (float)id->get_number();
  }

  return largest;
}

void Task::TaskManager::print_tasks() {
  auto list = this->parser->get_root()->get_list();
  std::cout << list.size() << std::endl;
  for (auto itr = list.begin(); itr != list.end(); itr++) {

    itr->get()->print_node(0);
  }
}

void Task::TaskManager::create_new_task(std::string *task) {
  parser->parse();

  float new_id = this->find_last_id() + 1;

  auto get_current_time = []() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

    return new std::string(buffer);
  };

  JSON::JSON_Object *object = new JSON::JSON_Object();
  (*object)["id"] = Task::NodeFactory::create_node<float>(&new_id);
  (*object)["description"] = Task::NodeFactory::create_node<std::string>(task);
  (*object)["status"] = Task::NodeFactory::create_node(new std::string("todo"));
  (*object)["created_at"] = Task::NodeFactory::create_node(get_current_time());
  (*object)["updated_at"] = Task::NodeFactory::create_node(get_current_time());
  std::shared_ptr<JSON::JSON_Node> object_node =
      std::make_shared<JSON::JSON_Node>();
  object_node->set_object(object);
  this->flush_new_node(object_node);
  std::clog << "Task created successfully" << std::endl;
}

// TODO: Try not to poke my eyes out next time when I see this....
// I am stupid I guess?!?!?!?!?!
void Task::TaskManager::flush_new_node(
    std::shared_ptr<JSON::JSON_Node> new_object_node) {
  auto list = this->parser->_root->get_list();
  JSON::JSON_List *new_list = new JSON::JSON_List();
  for (const auto &item : list) {
    new_list->push_back(item);
  }

  new_list->push_back(new_object_node);

  std::shared_ptr<JSON::JSON_Node> new_root_node =
      std::make_shared<JSON::JSON_Node>();
  new_root_node->set_list(new_list);

  this->parser->_root = new_root_node;
}
