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
  if (argc == 1) {
    std::cout << "create \"<your_task>\" -> creates new task" << std::endl;
    std::cout << "update <ID> \"<your_task>\" -> updates task" << std::endl;
    std::cout << "delete <ID> -> deletes task" << std::endl;
    std::cout << "mark-in-progress <ID> -> sets task in-progress" << std::endl;
    std::cout << "mark-done <ID> -> sets task done" << std::endl;
    std::cout << "list -> lists all tasks, you can specify status filter: "
                 "in-progress, todo and done"
              << std::endl;
    exit(0);
  }
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
  // If file doesn't exist simply create empty array json
  if (strcmp(argv[1], "--help") == 0) {
    std::cout << "create \"<your_task>\" -> creates new task" << std::endl;
    std::cout << "update <ID> \"<your_task>\" -> updates task" << std::endl;
    std::cout << "delete <ID> -> deletes task" << std::endl;
    std::cout << "mark-in-progress <ID> -> sets task in-progress" << std::endl;
    std::cout << "mark-done <ID> -> sets task done" << std::endl;
    std::cout << "list -> lists all tasks, you can specify status filter: "
                 "in-progress, todo and done"
              << std::endl;
  } else if (strcmp(argv[1], "add") == 0) {
    if (argc < 3) {
      std::cerr << "Cannot create empty task";
      exit(-1);
    }
    task_manager->create_new_task(new std::string(argv[2]));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "update") == 0) {
    if (argc < 4) {
      std::cerr << "You didn't set proper ID and Value to update task";
      exit(-1);
    }
    task_manager->update_task(atoi(argv[2]), new std::string(argv[3]));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "mark-in-progress") == 0) {
    if (argc < 3) {
      std::cerr << "You didn't set proper ID to update";
      exit(-1);
    }
    task_manager->update_status(atoi(argv[2]), new std::string("in-progress"));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "mark-done") == 0) {
    if (argc < 3) {
      std::cerr << "You didn't set proper ID to update";
      exit(-1);
    }
    task_manager->update_status(atoi(argv[2]), new std::string("done"));
    task_manager->write_to_file();
  } else if (strcmp(argv[1], "list") == 0) {
    if (argc >= 3) {
      task_manager->print_filtered_tasks(new std::string(argv[2]));
    } else {
      task_manager->print_tasks();
    }
  } else if (strcmp(argv[1], "delete") == 0) {
    if (argc < 3) {
      std::cerr << "You didn't set proper ID to delete";
      exit(-1);
    }
    task_manager->delete_task(atoi(argv[2]));
    task_manager->write_to_file();
  } else {
    std::cout << "create \"<your_task>\" -> creates new task" << std::endl;
    std::cout << "update <ID> \"<your_task>\" -> updates task" << std::endl;
    std::cout << "delete <ID> -> deletes task" << std::endl;
    std::cout << "mark-in-progress <ID> -> sets task in-progress" << std::endl;
    std::cout << "mark-done <ID> -> sets task done" << std::endl;
    std::cout << "list -> lists all tasks, you can specify status filter: "
                 "in-progress, todo and done"
              << std::endl;
  }

  delete task_manager;
  delete parser;
  return 0;
}
