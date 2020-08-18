#include <jsonx/JsonObject.h>
#include <jsonx/JsonX.h>

namespace jsonx {

/******************************************
Object::iterator
******************************************/

Object::iterator::iterator(Object &ref) : m_ref(ref), m_index(0) {}
Object::iterator::iterator(Object &ref, unsigned index)
    : m_ref(ref), m_index(index) {}
Object::iterator Object::iterator::operator++() {
  iterator i = *this;
  ++m_index;
  return i;
}
Object::iterator Object::iterator::operator++(int junk) {
  ++m_index;
  return *this;
}
Object &Object::iterator::operator*() { return second(); }
std::string Object::iterator::first() const { return m_ref.key(m_index); }
Object &Object::iterator::second() { return m_ref[m_ref.key(m_index)]; }
bool Object::iterator::operator==(const iterator &rhs) const {
  return &m_ref == &rhs.m_ref && m_index == rhs.m_index;
}
bool Object::iterator::operator!=(const iterator &rhs) const {
  return &m_ref != &rhs.m_ref || m_index != rhs.m_index;
}

/******************************************
Object::const_iterator
******************************************/
Object::const_iterator::const_iterator(Object const &ref)
    : m_ref(ref), m_index(0) {}
Object::const_iterator::const_iterator(Object const &ref, unsigned index)
    : m_ref(ref), m_index(index) {}
Object::const_iterator Object::const_iterator::operator++() {
  const_iterator i = *this;
  ++m_index;
  return i;
}
Object::const_iterator Object::const_iterator::operator++(int junk) {
  ++m_index;
  return *this;
}
Object Object::const_iterator::operator*() const { return second(); }
std::string Object::const_iterator::first() const { return m_ref.key(m_index); }
Object Object::const_iterator::second() const {
  return m_ref[m_ref.key(m_index)];
}
bool Object::const_iterator::operator==(const const_iterator &rhs) const {
  return &m_ref == &rhs.m_ref && m_index == rhs.m_index;
}
bool Object::const_iterator::operator!=(const const_iterator &rhs) const {
  return &m_ref != &rhs.m_ref || m_index != rhs.m_index;
}

/******************************************
Object
******************************************/
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

bool Object::operator==(const Object &rhs) const {

  if (this == &rhs)
    return true;

  if (m_type == rhs.m_type) {

    switch (m_type) {
    case Type::STRING:
      return std::any_cast<std::string>(m_value) ==
             std::any_cast<std::string>(rhs.m_value);
    case Type::BOOLEAN:
      return std::any_cast<bool>(m_value) == std::any_cast<bool>(rhs.m_value);
    case Type::NUMBER:
      return std::any_cast<double>(m_value) ==
             std::any_cast<double>(rhs.m_value);
    case Type::Null:
      return true;
    case Type::OBJECT: {

      if (count() != rhs.count())
        return false;

      auto keys = this->keys();
      auto const &map = std::any_cast<jobject const &>(m_value);
      auto const &rhsmap = std::any_cast<jobject const &>(rhs.m_value);
      for (auto &k : keys) {
        if (!rhs.exists(k) || (map.at(k) != rhsmap.at(k)))
          return false;
      }
      return true;
    }
    case Type::ARRAY: {

      if (count() != rhs.count())
        return false;

      auto sequence = this->sequence();
      auto const &array = std::any_cast<jarray const &>(m_value);
      auto const &rhsarray = std::any_cast<jarray const &>(rhs.m_value);

      for (auto &i : sequence) {
        if (array[i] != rhsarray[i])
          return false;
      }
      return true;
    }
    default:
      break;
    }
  }

  return false;
}

bool Object::operator!=(const Object &rhs) const { return !operator==(rhs); }

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

Object::const_iterator Object::begin() const { return const_iterator(*this); }
Object::iterator Object::begin() { return iterator(*this); }

Object::const_iterator Object::end() const {
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    return const_iterator(*this, map.size());
  }
  return const_iterator(*this, 0);
}

Object::iterator Object::end() {
  if (m_value.type() == typeid(jobject)) {
    auto &map = std::any_cast<jobject &>(m_value);
    return iterator(*this, map.size());
  }
  return iterator(*this, 0);
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

std::string Object::key(unsigned index) const {
  std::string key;
  if (m_value.type() == typeid(jobject)) {
    auto const &map = std::any_cast<jobject const &>(m_value);
    for (auto &e : map)
      if (index-- == 0)
        key = e.first;
  }

  return key;
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

void Object::remove(std::string const &key) {
  if (m_value.type() == typeid(jobject)) {
    auto &map = std::any_cast<jobject &>(m_value);
    if (auto found = map.find(key); found != map.end())
      map.erase(found);
  }
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

  std::string s;

  switch (m_type) {
  case Type::STRING:
    s = std::any_cast<std::string>(m_value);
    break;
  case Type::BOOLEAN:
    s = std::to_string(std::any_cast<bool>(m_value));
    break;
  case Type::NUMBER:
    s = std::to_string(std::any_cast<double>(m_value));
    break;
  case Type::Null:
    s = "null";
    break;
  case Type::OBJECT:
    s = stringify(*this);
    break;
  case Type::ARRAY:
    s = stringify(*this);
    break;
  default:
    break;
  }

  return s;
}

bool Object::toBoolean() const {

  bool b;

  switch (m_type) {
  case Type::BOOLEAN:
    b = std::any_cast<bool>(m_value);
    break;
  case Type::NUMBER:
    b = std::any_cast<double>(m_value) > 0;
    break;
  case Type::STRING:
    b = !std::any_cast<std::string>(m_value).empty();
    break;
  case Type::OBJECT:
    b = !std::any_cast<jobject const &>(m_value).empty();
    break;
  case Type::ARRAY:
    b = !std::any_cast<jarray const &>(m_value).empty();
    break;
  case Type::Null:
    b = false;
    break;
  default:
    break;
  }
  return b;
}

double Object::toNumber() const {

  double d;

  switch (m_type) {
  case Type::BOOLEAN:
    d = std::any_cast<bool>(m_value);
    break;
  case Type::NUMBER:
    d = std::any_cast<double>(m_value);
    break;
  case Type::STRING:
    d = std::stod(std::any_cast<std::string>(m_value).c_str());
    break;
  default:
    break;
  }

  return d;
}

std::vector<Object> Object::toArray() const {
  return m_value.type() == typeid(jarray) ? std::any_cast<jarray>(m_value)
                                          : jarray();
}

} // namespace jsonx
