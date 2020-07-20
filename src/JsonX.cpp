#include "Parser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <jsonx/JsonX.h>

namespace jsonx {

std::string stringify(Object const &obj) {
  ObjectParser parser;
  return parser(obj);
}

void write(Object const &obj, std::string const path) {
  std::string output = jsonx::stringify(obj);

  std::ofstream file(path, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file << output;
    file.close();
  }
}

std::optional<Object> objectify(std::string const& buffer) {
  StringParser parser;
  return parser(buffer);
}

} // namespace jsonx
