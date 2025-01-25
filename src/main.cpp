#include <string>

#include "Parser.h"

int main() {
  Parser *parser = new Parser("./sample-db.json");
  parser->parse();

  return 0;
}
