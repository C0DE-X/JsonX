#include "Parser.h"
#include <functional>
#include <map>

namespace jsonx {

std::string ObjectParser::operator()(Object const &obj) const {
  return parse(obj);
}

std::string ObjectParser::parse(Object const &obj) {
  static const std::map<Object::Type,
                        std::function<std::string(Object const &obj)>>
      table{{Object::Type::OBJECT, &ObjectParser::fromObject},
            {Object::Type::ARRAY, &ObjectParser::fromArray},
            {Object::Type::BOOLEAN, &ObjectParser::fromBoolean},
            {Object::Type::NUMBER, &ObjectParser::fromNumber},
            {Object::Type::Null, &ObjectParser::fromNull},
            {Object::Type::STRING, &ObjectParser::fromString}};

  auto iter = table.find(obj.type());

  return iter != table.end() ? iter->second(obj) : std::string();
}

std::string ObjectParser::fromObject(Object const &obj) {

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
std::string ObjectParser::fromArray(Object const &obj) {

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
std::string ObjectParser::fromNumber(Object const &obj) {

  std::string value = std::to_string(obj.toNumber());
  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  return value;
}
std::string ObjectParser::fromBoolean(Object const &obj) {
  return obj.toBoolean() ? "true" : "false";
}
std::string ObjectParser::fromString(Object const &obj) {
  return "\"" + obj.toString() + "\"";
}

std::string ObjectParser::fromNull(Object const &obj) {
  return "null";
}

StringParser::StringIterator::StringIterator(std::string const &s)
    : m_iter(s.begin()), m_end(s.end()) {}

char StringParser::StringIterator::operator*() const {
  return m_iter != m_end ? *m_iter : char();
}

bool StringParser::StringIterator::hasNext() const {
  return m_iter != m_end && std::next(m_iter) != m_end;
}

bool StringParser::StringIterator::next() {
  if (hasNext()) {
    ++m_iter;
    return true;
  }
  return false;
}

void StringParser::StringIterator::skipSpace() {
  while (hasNext() && std::isspace(*m_iter))
    next();
}

std::optional<Object>
StringParser::operator()(std::string const &buffer) const {
  StringIterator iter(buffer);
  return parse(iter);
}

std::optional<Object> StringParser::parse(StringIterator &iter) {

  static const std::map<char,
                        std::function<std::optional<Object>(StringIterator &)>>
      table{{'{', &StringParser::toObject},  {'[', &StringParser::toArray},
            {'"', &StringParser::toString},  {'.', &StringParser::toNumber},
            {'0', &StringParser::toNumber},  {'1', &StringParser::toNumber},
            {'2', &StringParser::toNumber},  {'3', &StringParser::toNumber},
            {'4', &StringParser::toNumber},  {'5', &StringParser::toNumber},
            {'6', &StringParser::toNumber},  {'7', &StringParser::toNumber},
            {'8', &StringParser::toNumber},  {'9', &StringParser::toNumber},
            {'t', &StringParser::toBoolean}, {'f', &StringParser::toBoolean},
            {'T', &StringParser::toBoolean}, {'F', &StringParser::toBoolean},
            {'N', &StringParser::toNull}, {'n', &StringParser::toNull}};

  std::optional<Object> obj;
  iter.skipSpace();
  if (iter.hasNext()) {
    if (auto found = table.find(*iter); found != table.end())
      obj = found->second(iter);
  }
  return obj;
}

std::optional<Object> StringParser::toObject(StringIterator &iter) {

  std::optional<Object> obj;

  if (*iter == '{') {
    obj = Object();
    bool lastKey = true;

    iter.next();
    iter.skipSpace();
    while (*iter != '}') {
      if (*iter == '"') {
        lastKey = true;
        std::string key;
        while (iter.next() && *iter != '"')
          key += *iter;

        if (key.empty() || *iter != '"' || !iter.next()) {
          obj.reset();
          break;
        }

        iter.skipSpace();
        if (*iter == ':') {
          iter.next();
          auto value = parse(iter);

          if (value) {
            obj->operator[](key) = value.value();
          } else {
            obj.reset();
            break;
          }

        } else {
          obj.reset();
          break;
        }

        iter.skipSpace();
        if (*iter == ',') {
          lastKey = false;
          iter.next();
          iter.skipSpace();
        }
      } else {
        obj.reset();
        break;
      }
    }

    if (*iter != '}' || !lastKey)
      obj.reset();
    else
      iter.next();
  }

  return obj;
}

std::optional<Object> StringParser::toArray(StringIterator &iter) {

  std::optional<Object> obj;

  if (*iter == '[') {
    obj = Object();
    *obj = std::vector<Object>();
    int counter = 0;
    bool lastKey = true;

    iter.next();
    iter.skipSpace();
    while (*iter != ']') {
      lastKey = true;
      auto value = parse(iter);
      if (value) {
        obj->operator[](counter) = value.value();
        ++counter;
      } else {
        obj.reset();
        break;
      }

      iter.skipSpace();
      if (*iter == ',') {
        lastKey = false;
        iter.next();
        iter.skipSpace();
      }
    }

    if (*iter != ']' || !lastKey)
      obj.reset();
    else
      iter.next();
  }

  return obj;
}

std::optional<Object> StringParser::toNumber(StringIterator &iter) {

  std::optional<Object> obj;

  std::string value;
  while (iter.hasNext() && isdigit(*iter)) {
    value += *iter;
    iter.next();
  }

  if (*iter == '.') {
    value += *iter;
    while (iter.next() && isdigit(*iter))
      value += *iter;
  }

  if (!value.empty()) {
    obj = Object();
    *obj = std::stod(value);
  }

  return obj;
}

std::optional<Object> StringParser::toBoolean(StringIterator &iter) {

  std::optional<Object> obj;

  std::string b;
  while (iter.hasNext() && std::isalpha(*iter)) {
    b += *iter;
    iter.next();
  }

  std::transform(b.begin(), b.end(), b.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (b == "false") {
    obj = Object();
    *obj = false;
  } else if (b == "true") {
    obj = Object();
    *obj = true;
  }

  return obj;
}

std::optional<Object> StringParser::toString(StringIterator &iter) {
  std::optional<Object> obj;
  if (*iter == '"') {
    std::string value;
    while (iter.next() && *iter != '"')
      value += *iter;

    if (*iter == '"') {
      obj = Object();
      *obj = value;
      iter.next();
    }
  }
  return obj;
}


std::optional<Object> StringParser::toNull(StringIterator &iter) {

  std::optional<Object> obj;

  std::string n;
  while (iter.hasNext() && std::isalpha(*iter)) {
    n += *iter;
    iter.next();
  }

  std::transform(n.begin(), n.end(), n.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (n == "null") {
    obj = Object();
    *obj = nullptr;
  }

  return obj;
}

} // namespace jsonx
