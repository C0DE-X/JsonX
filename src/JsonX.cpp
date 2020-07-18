#include <fstream>
#include <jsonx/JsonX.h>

namespace jsonx {

std::string Stringify::operator()(Object const &obj) const {
  std::string buffer;
  toString(obj, buffer);
  return buffer;
}

void Stringify::toString(Object const &obj, std::string &buffer) const {

  switch (obj.type()) {
  case Object::Type::OBJECT:
    fromObject(obj, buffer);
    break;
  case Object::Type::BOOLEAN:
    fromBoolean(obj, buffer);
    break;
  case Object::Type::NUMBER:
    fromNumber(obj, buffer);
    break;
  case Object::Type::STRING:
    fromString(obj, buffer);
  // case Object::Type::ARRAY:
  default:
    break;
  }
}

void Stringify::fromObject(Object const &obj, std::string &buffer) const {

  auto keys = obj.keys();

  buffer += "{";
  for (auto iter = keys.begin(); iter != keys.end(); ++iter) {

    buffer += "\"" + *iter + "\":";
    toString(obj[*iter], buffer);

    if (std::next(iter) != keys.end())
      buffer += ",";
  }

  buffer += "}";
}
void Stringify::fromNumber(Object const &obj, std::string &buffer) const {

  std::string value = std::to_string(obj.toNumber());
  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  buffer += value;
}
void Stringify::fromBoolean(Object const &obj, std::string &buffer) const {
  buffer += obj.toBoolean() ? "true" : "false";
}
void Stringify::fromString(Object const &obj, std::string &buffer) const {
  buffer += "\"" + obj.toString() + "\"";
}

Object Objectify::operator()(std::string const &buffer) const
{
    Object obj;

    return obj;
}

void Writer::operator()(Object const &obj, std::string const path) const {
  Stringify stringify;
  std::string output = stringify(obj);

  std::ofstream file(path, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file << output;
    file.close();
  }
}

} // namespace jsonx