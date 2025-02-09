#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>
#include <fstream>
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
  auto list = this->parser->_root->get_list();
  if (list.size() == 0)
    return 0;

  float largest = 0;
  for (auto itr = list.begin(); itr != list.end(); itr++) {
    auto object = itr->get()->get_object();
    auto id = object["id"];

    if (id->get_number() > largest)
      largest = (float)id->get_number();
  }

  return largest;
}

void Task::TaskManager::print_filtered_tasks(std::string *filter) {
  auto list = this->parser->_root->get_list();

  if (list.size() == 0) {
    return;
  }

  for (auto item : list) {
    auto object = item->get_object();
    std::string status = object["status"]->get_string();
    status.erase(std::remove_if(status.begin(), status.end(), ::isspace),
                 status.end());
    if (filter->compare(status) == 0) {
      std::cout << object["id"]->get_number() << std::setw(30)
                << object["description"]->get_string() << std::setw(30)
                << object["status"]->get_string() << std::setw(30)
                << object["created_at"]->get_string() << std::setw(30)
                << object["updated_at"]->get_string() << std::endl;
    }
  }
}

void Task::TaskManager::print_tasks() {
  auto list = this->parser->_root->get_list();

  if (list.size() == 0) {
    return;
  }

  for (auto item : list) {
    auto object = item->get_object();
    std::cout << object["id"]->get_number() << std::setw(30)
              << object["description"]->get_string() << std::setw(30)
              << object["status"]->get_string() << std::setw(30)
              << object["created_at"]->get_string() << std::setw(30)
              << object["updated_at"]->get_string() << std::endl;
  }
}

void Task::TaskManager::create_new_task(std::string *task) {
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

void Task::TaskManager::update_status(const int &id, std::string *status) {
  auto list = this->parser->_root;
  auto &vec = list->get_list();

  for (auto &item : vec) {
    if (item && item->get_object()["id"]->get_number() == id) {
      auto &object = item->get_object();
      std::clog << "Updating status from " << object["status"]->get_string()
                << " to " << (*status) << " for id " << id << std::endl;
      object["status"] = Task::NodeFactory::create_node(status);
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

void Task::TaskManager::write_to_file() {
  std::ofstream file;
  file.open(FILE_PATH, std::fstream::out | std::fstream::trunc);
  file << this->parser->_root->to_string(0);
}
