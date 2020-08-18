#pragma once

#include <any>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace jsonx {

class JsonIterator;

class Object {

  using jobject = std::unordered_map<std::string, Object>;
  using jarray = std::vector<Object>;

public:
  enum Type { Null, STRING, NUMBER, BOOLEAN, OBJECT, ARRAY };

  class iterator {
    friend class Object;

  public:
    iterator operator++();
    iterator operator++(int junk);
    Object &operator*();
    std::string first() const;
    Object &second();
    bool operator==(const iterator &rhs) const;
    bool operator!=(const iterator &rhs) const;

  private:
    iterator(Object &ref);
    iterator(Object &ref, unsigned index);

    Object &m_ref;
    unsigned m_index;
  };

  class const_iterator {
    friend class Object;

  public:
    const_iterator operator++();
    const_iterator operator++(int junk);
    Object operator*() const;
    std::string first() const;
    Object second() const;
    bool operator==(const const_iterator &rhs) const;
    bool operator!=(const const_iterator &rhs) const;

  private:
    const_iterator(Object const &ref);
    const_iterator(Object const &ref, unsigned index);

    Object const &m_ref;
    unsigned m_index;
  };

  Object() = default;
  Object(Object const &other);
  Object(Object &&other);
  Object(std::string const &value);
  Object(const char *value);
  Object(bool const &value);
  Object(short const &value);
  Object(int const &value);
  Object(float const &value);
  Object(double const &value);
  Object(std::vector<Object> const &value);
  Object(std::nullptr_t value);
  ~Object() = default;

  Object &operator=(Object const &other);
  Object &operator=(Object &&other);
  Object &operator=(std::string const &value);
  Object &operator=(const char *value);
  Object &operator=(bool const &value);
  Object &operator=(short const &value);
  Object &operator=(int const &value);
  Object &operator=(float const &value);
  Object &operator=(double const &value);
  Object &operator=(std::vector<Object> const &value);
  Object &operator=(std::nullptr_t value);

  bool operator==(const Object &rhs) const;
  bool operator!=(const Object &rhs) const;

  Object operator[](std::string const &key) const;
  Object &operator[](std::string const &key);
  Object operator[](int const &index) const;
  Object &operator[](int const &index);

  Object::const_iterator begin() const;
  Object::iterator begin();
  Object::const_iterator end() const;
  Object::iterator end();

  Type type() const;
  bool isObject() const;
  bool isArray() const;
  bool isString() const;
  bool isNumber() const;
  bool isBoolean() const;
  bool isNull() const;

  size_t count() const;
  std::vector<std::string> keys() const;
  std::string key(unsigned index) const;
  std::vector<int> sequence() const;
  bool exists(std::string const &key) const;
  void add(std::string const &key, Object const &obj);
  void add(Object const &obj);
  void remove(std::string const &key);
  void remove(int const &index);
  void clear();

  std::string toString() const;
  bool toBoolean() const;
  double toNumber() const;
  std::vector<Object> toArray() const;

private:
  std::any m_value{jobject()};
  Type m_type{Type::OBJECT};
};

} // namespace jsonx
