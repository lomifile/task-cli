#include "JSON.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
  std::cout << "hello world" << std::endl;
  std::ifstream db;
  db.open("sample-db.json");
  std::string data;

  if (!db.is_open()) {
    std::cout << "Error while opening file" << std::endl;
    return -1;
  }

  while (db) {
    std::getline(db, data);
    std::cout << data << std::endl;
  }

  return 0;
}
