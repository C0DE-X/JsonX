#include <jsonx/JsonObject.h>

namespace jsonx {

Object::Object(Object const &other)
    : m_value(other.m_value), m_type(other.m_type) {}

Object::Object(Object &&other) : m_value(other.m_value), m_type(other.m_type) {}

Object &Object::operator=(Object const &other) {
  m_value = other.m_value;
  m_type = other.m_type;
  return *this;
}
Object &Object::operator=(Object &&other) {
  m_value = other.m_value;
  m_type = other.m_type;

  other.m_value = object();
  other.m_type = Type::OBJECT;
  return *this;
}
Object::Type Object::type() const { return m_type; }

bool Object::exists(std::string const &key) const {
  if (m_value.type() == typeid(object)) {
    auto const &map = std::any_cast<object const &>(m_value);
    return map.find(key) != map.end();
  }
  return false;
}

void Object::remove(std::string const &key) {
  if (m_value.type() == typeid(object)) {
    auto &map = std::any_cast<object &>(m_value);
    map.erase(key);
  }
}

void Object::clear()
{
  m_value = object();
  m_type = Type::OBJECT;
}

std::vector<std::string> Object::keys() const {
  std::vector<std::string> keys;
  if (m_value.type() == typeid(object)) {
    auto const &map = std::any_cast<object const &>(m_value);
    for (auto &e : map)
      keys.push_back(e.first);
  }

  return keys;
}

Object Object::operator[](std::string const &key) const {
  if (m_value.type() == typeid(object)) {
    auto const &map = std::any_cast<object const &>(m_value);
    return map.find(key) != map.end() ? map.at(key) : Object();
  }

  return Object();
}

Object &Object::operator[](std::string const &key) {
  if (m_value.type() != typeid(object)) {
    m_type = Type::OBJECT;
    m_value = object();
  }

  return std::any_cast<object &>(m_value)[key];
}

std::string Object::toString() const {

  switch (m_type) {
  case Type::STRING:
    return std::any_cast<std::string>(m_value);
  case Type::OBJECT:
    // TODO
    return "{}";
  case Type::ARRAY:
    // TODO
    return "[]";
  case Type::BOOLEAN:
    return std::to_string(std::any_cast<bool>(m_value));
  case Type::NUMBER:
    return std::to_string(std::any_cast<double>(m_value));
  default:
    return std::string();
  }
}

bool Object::toBoolean() const {
  return m_type == Type::BOOLEAN ? std::any_cast<bool>(m_value) : bool();
}

double Object::toNumber() const {
  return m_type == Type::NUMBER ? std::any_cast<double>(m_value) : double();
}

Object &Object::operator=(std::string const &value) {
  m_type = Type::STRING;
  m_value = value;
  return *this;
}

Object &Object::operator=(const char *value) {
  m_type = Type::STRING;
  m_value = std::string(value);
  return *this;
}

Object &Object::operator=(bool const &value) {
  m_type = Type::BOOLEAN;
  m_value = value;
  return *this;
}

Object &Object::operator=(short const &value) {
  m_type = Type::NUMBER;
  m_value = double(value);
  return *this;
}

Object &Object::operator=(int const &value) {
  m_type = Type::NUMBER;
  m_value = double(value);
  return *this;
}

Object &Object::operator=(float const &value) {
  m_type = Type::NUMBER;
  m_value = double(value);
  return *this;
}

Object &Object::operator=(double const &value) {
  m_type = Type::NUMBER;
  m_value = value;
  return *this;
}

} // namespace jsonx
