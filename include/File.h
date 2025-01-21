#include <fstream>
#include <string>

class FileHandler {
  FileHandler(std::string &path) : path(path) { this->file.open(path); }
  ~FileHandler() {}

  inline std::string *getLine() {
    std::string value;
    std::getline(this->file, value);
    return &value;
  }

private:
  std::string path;
  std::ifstream file;
};
