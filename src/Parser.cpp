#include "Parser.h"
#include <functional>
#include <map>

namespace jsonx {

std::string Parser::operator()(Object const &obj) const { return parse(obj); }

std::string Parser::parse(Object const &obj) {
  static const std::map<Object::Type,
                        std::function<std::string(Object const &obj)>>
      table{{Object::Type::OBJECT, &Parser::fromObject},
            {Object::Type::ARRAY, &Parser::fromArray},
            {Object::Type::BOOLEAN, &Parser::fromBoolean},
            {Object::Type::NUMBER, &Parser::fromNumber},
            {Object::Type::STRING, &Parser::fromString}};

  auto iter = table.find(obj.type());

  return iter != table.end() ? iter->second(obj) : std::string();
}

std::string Parser::fromObject(Object const &obj) {

  std::string buffer;

  auto keys = obj.keys();
  buffer += "{";
  for (auto iter = keys.begin(); iter != keys.end(); ++iter) {
    buffer += "\"" + *iter + "\":";
    buffer += parse(obj[*iter]);
    if (std::next(iter) != keys.end())
      buffer += ",";
  }
  buffer += "}";

  return buffer;
}
std::string Parser::fromArray(Object const &obj) {

  std::string buffer;

  auto size = obj.count();
  buffer += "[";
  for (int i = 0; i < size; ++i) {
    buffer += parse(obj[i]);
    if (i < (size - 1))
      buffer += ",";
  }
  buffer += "]";

  return buffer;
}
std::string Parser::fromNumber(Object const &obj) {

  std::string value = std::to_string(obj.toNumber());
  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  return value;
}
std::string Parser::fromBoolean(Object const &obj) {
  return obj.toBoolean() ? "true" : "false";
}
std::string Parser::fromString(Object const &obj) {
  return "\"" + obj.toString() + "\"";
}
} // namespace jsonx