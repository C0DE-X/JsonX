#include <jsonx/JsonObject.h>

namespace jsonx {

Object::Object(Object const &other)
    : m_value(other.m_value), m_type(other.m_type) {}

Object::Object(Object &&other) : m_value(other.m_value), m_type(other.m_type) {}

Object::Object(std::string const &value)
    : m_value(value), m_type(Type::STRING) {}

Object::Object(const char *value)
    : m_value(std::string(value)), m_type(Type::STRING) {}

Object::Object(bool const &value) : m_value(value), m_type(Type::BOOLEAN) {}

Object::Object(short const &value)
    : m_value(double(value)), m_type(Type::NUMBER) {}

Object::Object(int const &value)
    : m_value(double(value)), m_type(Type::NUMBER) {}

Object::Object(float const &value)
    : m_value(double(value)), m_type(Type::NUMBER) {}

Object::Object(double const &value) : m_value(value), m_type(Type::NUMBER) {}

Object::Object(std::vector<Object> const &value)
    : m_value(value), m_type(Type::ARRAY) {}

Object::Object(std::nullptr_t value) : m_value(value), m_type(Type::Null) {}

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

Object &Object::operator=(std::vector<Object> const &value) {
  m_type = Type::ARRAY;
  m_value = value;
  return *this;
}

Object &Object::operator=(std::nullptr_t value) {
  m_type = Type::Null;
  m_value = nullptr;
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

std::map<std::string, Object>::const_iterator Object::begin() const {
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    return map.begin();
  }
  return jobject::const_iterator();
}

std::map<std::string, Object>::iterator Object::begin() {
  if (m_value.type() == typeid(jobject)) {
    auto &map = std::any_cast<jobject &>(m_value);
    return map.begin();
  }
  return jobject::iterator();
}

std::map<std::string, Object>::const_iterator Object::end() const {
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    return map.end();
  }
  return jobject::const_iterator();
}

std::map<std::string, Object>::iterator Object::end() {
  if (m_value.type() == typeid(jobject)) {
    auto &map = std::any_cast<jobject &>(m_value);
    return map.end();
  }
  return jobject::iterator();
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

bool Object::isObject() const { return m_type == Type::OBJECT; }

bool Object::isArray() const { return m_type == Type::ARRAY; }

bool Object::isString() const { return m_type == Type::STRING; }

bool Object::isNumber() const { return m_type == Type::NUMBER; }

bool Object::isBoolean() const { return m_type == Type::BOOLEAN; }

bool Object::isNull() const { return m_type == Type::Null; }

std::vector<std::string> Object::keys() const {
  std::vector<std::string> keys;
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    for (auto &e : map)
      keys.push_back(e.first);
  }

  return keys;
}

std::vector<int> Object::sequence() const {
  std::vector<int> seq;

  if (m_value.type() == typeid(jarray)) {
    for (int i = 0; i < count(); ++i)
      seq.push_back(i);
  }

  return seq;
}

bool Object::exists(std::string const &key) const {
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    return map.find(key) != map.end();
  }
  return false;
}

void Object::add(std::string const &key, Object const &obj) {
  if (m_value.type() != typeid(jobject)) {
    m_type = Type::OBJECT;
    m_value = jobject();
  }
  auto &map = std::any_cast<jobject &>(m_value);
  map.insert({key, obj});
}

void Object::add(Object const &obj) {
  if (m_value.type() != typeid(jarray)) {
    m_type = Type::ARRAY;
    m_value = jarray();
  }
  auto &array = std::any_cast<jarray &>(m_value);
  array.push_back(obj);
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
