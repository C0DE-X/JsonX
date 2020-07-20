#pragma once

#include <jsonx/JsonObject.h>

namespace jsonx {

std::string stringify(Object const &obj);
void write(Object const &obj, std::string const path);

class Objectify {
public:
  Object operator()(std::string const &buffer) const;

private:
  static Object fromString(std::string& buffer);
  static Object toObject(std::string& buffer);
  static Object toArray(std::string& buffer);
  static Object toNumber(std::string& buffer);
  static Object toBoolean(std::string& buffer);
  static Object toString(std::string& buffer);

  static void skipWhiteSpaces(std::string& buffer);
};

} // namespace jsonx