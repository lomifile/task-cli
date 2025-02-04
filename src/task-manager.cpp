#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "Node.h"
#include "TaskManager.h"

std::string *Task::TaskManager::get_current_time() {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);

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
  auto &list = this->parser->get_root()->get_list();

  std::cout << "| Id " << std::setw(10) << "|" << std::setw(20) << " Task"
            << std::setw(20) << "|" << std::setw(15) << "Status"
            << std::setw(14) << "|" << std::setw(15) << "Created at"
            << std::setw(18) << "|" << std::setw(15) << "Updated at"
            << std::setw(18) << "|" << std::endl;
  std::cout << "+-------------+"
            << "---------------------------------------+"
            << "----------------------------+"
            << "--------------------------------+"
            << "--------------------------------+" << std::endl;
  for (const auto &item : list) {
    auto object = item->get_object();
    std::cout << "|" << std::setw(5) << object["id"]->get_number()
              << std::setw(9) << "|" << std::setw(7)
              << object["description"]->get_string() << std::setw(33) << "|"
              << std::setw(14) << object["status"]->get_string()
              << std::setw(15) << "|" << std::setw(23)
              << object["created_at"]->get_string() << std::setw(10) << "|"
              << std::setw(23) << object["updated_at"]->get_string()
              << std::setw(10) << "|" << std::endl;
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
  JSON::JSON_List *new_list = new JSON::JSON_List(list);

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
  auto list = this->parser->_root;
  auto &vec = list->get_list();

  for (auto &item : vec) {
    if (item && item->get_object()["id"]->get_number() == id) {
      std::clog << "Updating task with id: " << id << std::endl;
      auto &object = item->get_object();
      object["description"] = Task::NodeFactory::create_node(task);
      break;
    }
  }

  this->parser->_root = list;

  std::clog << "Task updated successfully" << std::endl;
}

void Task::TaskManager::delete_task(const int &id) {
  auto &list = this->parser->_root->get_list();
  for (auto itr = list.begin(); itr != list.end(); ++itr) {
    if (itr->get()->get_object()["id"]->get_number() == id) {
      list.erase(itr);
      break;
    }
  }

  std::shared_ptr<JSON::JSON_Node> new_root_node =
      std::make_shared<JSON::JSON_Node>();
  new_root_node->set_list(&list);
  this->parser->_root = new_root_node;

  std::clog << "Item " << id << " deleted successfully" << std::endl;
}
