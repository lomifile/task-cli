#include <cstring>
#include <ctime>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "Node.h"
#include "TaskManager.h"

std::string *Task::TaskManager::get_current_time() {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

  return new std::string(buffer);
}

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

  JSON::JSON_Object *object = new JSON::JSON_Object();
  (*object)["id"] = Task::NodeFactory::create_node<float>(&new_id);
  (*object)["description"] = Task::NodeFactory::create_node<std::string>(task);
  (*object)["status"] = Task::NodeFactory::create_node(new std::string("todo"));
  (*object)["created_at"] =
      Task::NodeFactory::create_node(this->get_current_time());
  (*object)["updated_at"] =
      Task::NodeFactory::create_node(this->get_current_time());
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

JSON::JSON_Node *Task::TaskManager::find_node(const int &id) {
  JSON::JSON_List list = this->parser->_root->get_list();
  for (auto itr = list.begin(); itr != list.end(); ++itr) {
    auto node = itr->get();
    if (node->get_object()["id"].get()->get_number() == id) {
      return node;
    }
  }

  return nullptr;
}

void Task::TaskManager::update_task(const int &id, std::string *task) {
  auto find_idx = [this](int id) {
    auto list = this->parser->_root->get_list();
    for (int idx = 0; idx < list.size(); idx++) {
      auto element = list[idx]->get_object()["id"]->get_number();
      if (element == id)
        return idx;
    }
    return -1;
  };

  auto prev = this->find_node(id);
  auto object = prev->get_object();

  JSON::JSON_Object *updated_object = new JSON::JSON_Object();
  (*updated_object)["id"] = object["id"];
  (*updated_object)["description"] = Task::NodeFactory::create_node(task);
  (*updated_object)["status"] = object["status"];
  (*updated_object)["created_at"] = object["created_at"];
  (*updated_object)["updated_at"] =
      Task::NodeFactory::create_node(this->get_current_time());

  std::shared_ptr<JSON::JSON_Node> new_object_node =
      std::make_shared<JSON::JSON_Node>();
  new_object_node->set_object(updated_object);

  JSON::JSON_List *updated_list = new JSON::JSON_List();
  (*updated_list) = this->parser->_root->get_list();
  int idx = find_idx(id);
  (*updated_list)[idx] = new_object_node;
  std::shared_ptr<JSON::JSON_Node> new_root_node =
      std::make_shared<JSON::JSON_Node>();
  new_root_node->set_list(updated_list);
  this->parser->_root = new_root_node;

  std::clog << "Task updated successfully" << std::endl;
}
