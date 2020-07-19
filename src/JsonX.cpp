#include <fstream>
#include <jsonx/JsonX.h>
#include <cctype>
#include <algorithm>

namespace jsonx {

std::string Stringify::operator()(Object const &obj) const {
  return toString(obj);
}

std::string Stringify::toString(Object const &obj) const {

  std::string buffer;

  switch (obj.type()) {
  case Object::Type::OBJECT:
    buffer = fromObject(obj);
    break;
  case Object::Type::BOOLEAN:
    buffer = fromBoolean(obj);
    break;
  case Object::Type::NUMBER:
    buffer = fromNumber(obj);
    break;
  case Object::Type::STRING:
    buffer = fromString(obj);
  // case Object::Type::ARRAY:
  default:
    break;
  }

  return buffer;
}

std::string Stringify::fromObject(Object const &obj) const {

  std::string buffer;

  auto keys = obj.keys();

  buffer += "{";
  for (auto iter = keys.begin(); iter != keys.end(); ++iter) {

    buffer += "\"" + *iter + "\":";
    buffer += toString(obj[*iter]);

    if (std::next(iter) != keys.end())
      buffer += ",";
  }

  buffer += "}";

  return buffer;
}

std::string Stringify::fromNumber(Object const &obj) const {

  std::string value = std::to_string(obj.toNumber());
  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  return value;
}

std::string Stringify::fromBoolean(Object const &obj) const {
  return obj.toBoolean() ? "true" : "false";
}

std::string Stringify::fromString(Object const &obj) const {
  return "\"" + obj.toString() + "\"";
}

Object Objectify::operator()(std::string const &buffer) const {

  std::string oBuffer(buffer);
  return fromString(oBuffer);
}

Object Objectify::fromString(std::string &buffer) const {
  Object obj;
  skipWhiteSpaces(buffer);

  if (!buffer.empty()) {
    switch (buffer.front()) {
    case '{':
      obj = toObject(buffer);
      break;
    case '"':
      obj = toString(buffer);
      break;
    case '.':
      obj = toNumber(buffer);
      break;
    case 't':
      obj = toBoolean(buffer);
      break;
    case 'f':
      obj = toBoolean(buffer);
      break;
    case 'T':
      obj = toBoolean(buffer);
      break;
    case 'F':
      obj = toBoolean(buffer);
      break;
    default:
      if (std::isdigit(buffer.front()))
        obj = toNumber(buffer);
      break;
    }
  }

  return obj;
}

Object Objectify::toObject(std::string &buffer) const {
  Object obj;

  if (buffer.front() == '{')
    buffer.erase(buffer.begin());

  while (!buffer.empty() && buffer.front() != '}') {

    skipWhiteSpaces(buffer);

    if (buffer.front() == '"') {
      buffer.erase(buffer.begin());
      std::string key;

      while (buffer.front() != '"') {
        key += buffer.front();
        buffer.erase(buffer.begin());
      }

      if (!buffer.empty())
        buffer.erase(buffer.begin());

      if (buffer.front() == ':') {
        buffer.erase(buffer.begin());
        Object value = fromString(buffer);
        obj[key] = value;
      }

      if (buffer.front() == ',')
        buffer.erase(buffer.begin());
    }
  }

  if (!buffer.empty() && buffer.front() == '}')
    buffer.erase(buffer.begin());

  return obj;
}

Object Objectify::toNumber(std::string &buffer) const {
  Object obj;
  skipWhiteSpaces(buffer);

  std::string value;
  while (!buffer.empty() && isdigit(buffer.front())) {
    value += buffer.front();
    buffer.erase(buffer.begin());
  }

  if (buffer.front() == '.') {
    value += buffer.front();
    buffer.erase(buffer.begin());

    while (!buffer.empty() && isdigit(buffer.front())) {
      value += buffer.front();
      buffer.erase(buffer.begin());
    }
  }

  if (!value.empty())
    obj = std::stod(value);

  skipWhiteSpaces(buffer);
  return obj;
}

Object Objectify::toBoolean(std::string &buffer) const {
  Object obj;
  skipWhiteSpaces(buffer);

  std::string b;
  while(!buffer.empty() && std::isalpha(buffer.front()))
  {
    b += buffer.front();
    buffer.erase(buffer.begin());
  }

  std::transform(b.begin(), b.end(), b.begin(),[](unsigned char c){ return std::tolower(c); });

  if(b == "false")
  {
    obj = false;
  }else if(b == "true")
  {
    obj = true;
  }

  skipWhiteSpaces(buffer);
  return obj;
}

Object Objectify::toString(std::string &buffer) const {
  Object obj;
  skipWhiteSpaces(buffer);

  if (buffer.front() == '"') {
    buffer.erase(buffer.begin());
    std::string value;
    while (!buffer.empty() && buffer.front() != '"') {
      value += buffer.front();
      buffer.erase(buffer.begin());
    }

    if (buffer.front() == '"') {
      obj = value;
      buffer.erase(buffer.begin());
    }
  }
  skipWhiteSpaces(buffer);
  return obj;
}

void Objectify::skipWhiteSpaces(std::string &buffer) const {
  while (!buffer.empty() && std::isspace(buffer.front()))
    buffer.erase(buffer.begin());
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
