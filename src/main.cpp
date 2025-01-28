#include <iostream>
#include <memory>
#include <string>

#include "Node.h"
#include "Parser.h"
#include "TaskManager.h"

int main() {
  Task::TaskManager *task_manager = new Task::TaskManager();
  task_manager->create_new_task(new std::string("Task 2"));
  task_manager->print_tasks();

  delete task_manager;
  return 0;
}
