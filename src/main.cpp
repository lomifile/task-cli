#include <iostream>
#include <string>

#include "Parser.h"

int main() {
  Parser *parser = new Parser("./sample-db.json");
  parser->parse();

  auto json = parser->get_root();

  return 0;
}
