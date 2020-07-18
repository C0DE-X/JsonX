#pragma once

#include <any>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace jsonx {

class Object {

  using object = std::map<std::string, Object>;

public:
  enum Type { STRING, NUMBER, BOOLEAN, OBJECT, ARRAY };

  Object() = default;
  Object(Object const& other);
  Object(Object&& other);
  ~Object() = default;

  Object& operator=(Object const& other);
  Object& operator=(Object&& other);

  Object operator[](std::string const &key) const;
  Object &operator[](std::string const &key);

  Type type() const;

  bool exists(std::string const &key) const;
  std::vector<std::string> keys() const;
  void remove(std::string const &key);
  void clear();

  std::string toString() const;
  bool toBoolean() const;
  double toNumber() const;

  Object &operator=(std::string const &value);
  Object &operator=(const char *value);
  Object &operator=(bool const &value);
  Object &operator=(short const &value);
  Object &operator=(int const &value);
  Object &operator=(float const &value);
  Object &operator=(double const &value);

private:
  std::any m_value{std::map<std::string, Object>()};
  Type m_type{Type::OBJECT};
};

} // namespace jsonx
