#include <algorithm>
#include <cctype>
#include <fstream>
#include <jsonx/JsonX.h>
#include "Parser.h"

namespace jsonx {

std::string stringify(Object const &obj)
{
  Parser parser;
  return parser(obj);
}

void write(Object const &obj, std::string const path)
{
  std::string output = jsonx::stringify(obj);

  std::ofstream file(path, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file << output;
    file.close();
  }
}

Object Objectify::operator()(std::string const &buffer) const {
  std::string oBuffer(buffer);
  return fromString(oBuffer);
}

Object Objectify::fromString(std::string &buffer) {

  static const std::map<char, std::function<Object(std::string&)>> table
  {
    {'{', &Objectify::toObject},
    {'[', &Objectify::toArray},
    {'"', &Objectify::toString},
    {'.', &Objectify::toNumber},
    {'t', &Objectify::toBoolean},
    {'f', &Objectify::toBoolean},
    {'T', &Objectify::toBoolean},
    {'F', &Objectify::toBoolean}
  };

  Object obj;
  skipWhiteSpaces(buffer);

  if (!buffer.empty()) {
    if(auto iter = table.find(buffer.front()); iter != table.end())
    {
      obj = iter->second(buffer);
    }else if (std::isdigit(buffer.front()))
        obj = toNumber(buffer);
  }

  return obj;
}

Object Objectify::toObject(std::string &buffer) {
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

Object Objectify::toArray(std::string& buffer)
{
  Object obj;

  if (buffer.front() == '[')
    buffer.erase(buffer.begin());

  int iter = 0;

  while (!buffer.empty() && buffer.front() != ']') {

    skipWhiteSpaces(buffer);

    Object value = fromString(buffer);
    obj[iter] = value;

    if (buffer.front() == ',')
      buffer.erase(buffer.begin());
    
    ++iter;
    skipWhiteSpaces(buffer);
  }

  if (!buffer.empty() && buffer.front() == ']')
    buffer.erase(buffer.begin());

  return obj;
}

Object Objectify::toNumber(std::string &buffer) {
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

Object Objectify::toBoolean(std::string &buffer) {
  Object obj;
  skipWhiteSpaces(buffer);

  std::string b;
  while (!buffer.empty() && std::isalpha(buffer.front())) {
    b += buffer.front();
    buffer.erase(buffer.begin());
  }

  std::transform(b.begin(), b.end(), b.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (b == "false") {
    obj = false;
  } else if (b == "true") {
    obj = true;
  }

  skipWhiteSpaces(buffer);
  return obj;
}

Object Objectify::toString(std::string &buffer) {
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

void Objectify::skipWhiteSpaces(std::string &buffer) {
  while (!buffer.empty() && std::isspace(buffer.front()))
    buffer.erase(buffer.begin());
}

} // namespace jsonx
