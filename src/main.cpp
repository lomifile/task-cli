#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "Node.h"
#include "Parser.h"
#include "TaskManager.h"

int main(int argc, char *argv[]) {
  // If file doesn't exist simply create empty array json
  Parser *parser = new Parser(FILE_PATH);
  if (!std::filesystem::exists(FILE_PATH)) {
    std::ofstream file;
    file.open(FILE_PATH, std::fstream::out | std::fstream::trunc);
    if (file.is_open()) {
      file.close();
    } else {
      std::cerr << "Error while opening " << FILE_PATH << std::endl;
    }
    JSON::JSON_List *new_list = new JSON::JSON_List();
    auto new_node = std::make_shared<JSON::JSON_Node>();
    new_node->set_list(new_list);
    parser->_root = new_node;
  } else {
    parser->parse();
  }

  Task::TaskManager *task_manager = new Task::TaskManager(parser);

  if (strcmp(argv[1], "--help") == 0) {
    std::cout << "Help" << std::endl;
  } else if (strcmp(argv[1], "add") == 0) {
    auto item = std::string(argv[2]);
    task_manager->create_new_task(&item);
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "update") == 0) {
    task_manager->update_task(atoi(argv[2]), new std::string(argv[3]));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "mark-in-progress") == 0) {
    task_manager->update_status(atoi(argv[2]), new std::string("in-progress"));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "mark-done") == 0) {
    task_manager->update_status(atoi(argv[2]), new std::string("done"));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "list") == 0) {
    task_manager->print_tasks();
  } else if (strcmp(argv[1], "delete") == 0) {
    task_manager->delete_task(atoi(argv[2]));
    task_manager->write_to_file();
  } else {
    std::cout << "You are missig sometihng" << std::endl;
  }

  delete task_manager;
  delete parser;
  return 0;
}
