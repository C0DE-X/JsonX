#include "Parser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <jsonx/JsonX.h>

std::optional<jsonx::Object> operator"" _jsonx(const char *buffer,
                                               long unsigned int size) {
  std::string s(buffer, size);
  return jsonx::objectify(s);
}

std::ostream& operator<<(std::ostream& os, const jsonx::Object& obj)
{
  os << jsonx::stringify(obj);
  return os;
}

namespace jsonx {

const Object Array() { return std::vector<Object>(); }

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

std::optional<Object> objectify(std::string const &buffer) {
  StringParser parser;
  return parser(buffer);
}

std::optional<Object> read(std::string const path) {
  std::string buffer;
  std::ifstream myfile(path);
  if (myfile.is_open()) {
    while (getline(myfile, buffer))
      ;
    myfile.close();
  }

  if (!buffer.empty())
    return objectify(buffer);

  return std::optional<Object>();
}

} // namespace jsonx
