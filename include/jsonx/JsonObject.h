#pragma once

#include <any>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace jsonx {

class Object {

  using jobject = std::map<std::string, Object>;
  using jarray = std::vector<Object>;

public:
  enum Type { STRING, NUMBER, BOOLEAN, OBJECT, ARRAY };

  Object() = default;
  Object(Object const &other);
  Object(Object &&other);
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

  Object operator[](std::string const &key) const;
  Object &operator[](std::string const &key);
  Object operator[](int const &index) const;
  Object &operator[](int const &index);

  Type type() const;
  size_t count() const;
  std::vector<std::string> keys() const;
  bool exists(std::string const &key) const;
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
