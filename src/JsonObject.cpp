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

  other.m_value = jobject();
  other.m_type = Type::OBJECT;
  return *this;
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

Object Object::operator[](std::string const &key) const {
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    return map.find(key) != map.end() ? map.at(key) : Object();
  }

  return Object();
}

Object &Object::operator[](std::string const &key) {
  if (m_value.type() != typeid(jobject)) {
    m_type = Type::OBJECT;
    m_value = jobject();
  }

  return std::any_cast<jobject &>(m_value)[key];
}

Object Object::operator[](int const &index) const {
  if (m_value.type() == typeid(jarray)) {
    auto const &array = std::any_cast<jarray const &>(m_value);
    return array.size() > index ? array.at(index) : Object();
  }

  return Object();
}
Object &Object::operator[](int const &index) {
  if (m_value.type() != typeid(jarray)) {
    m_type = Type::ARRAY;
    m_value = jarray();
  }

  auto &array = std::any_cast<jarray &>(m_value);

  if (array.size() <= index)
    array.resize(index + 1);

  return array.at(index);
}

Object::Type Object::type() const { return m_type; }

size_t Object::count() const {
  switch (m_type) {
  case Type::OBJECT:
    return std::any_cast<jobject const &>(m_value).size();
  case Type::ARRAY:
    return std::any_cast<jarray const &>(m_value).size();
  default:
    break;
  }

  return 0;
}

std::vector<std::string> Object::keys() const {
  std::vector<std::string> keys;
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    for (auto &e : map)
      keys.push_back(e.first);
  }

  return keys;
}

bool Object::exists(std::string const &key) const {
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    return map.find(key) != map.end();
  }
  return false;
}

void Object::remove(const int &index) {
  if (m_value.type() == typeid(jarray)) {
    auto &array = std::any_cast<jarray &>(m_value);
    if (array.size() > index)
      array.erase(array.begin() + index);
  }
}

void Object::clear() {
  m_value = jobject();
  m_type = Type::OBJECT;
}

std::string Object::toString() const {

  switch (m_type) {
  case Type::STRING:
    return std::any_cast<std::string>(m_value);
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

std::vector<Object> Object::toArray() const {
  return m_value.type() == typeid(jarray) ? std::any_cast<jarray>(m_value)
                                          : jarray();
}

} // namespace jsonx
