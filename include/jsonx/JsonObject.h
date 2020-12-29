#pragma once

#include <any>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace jsonx {

class Object {

  using jobject = std::unordered_map<std::string, Object>;
  using jarray = std::vector<Object>;

public:
  enum Type { Null, STRING, NUMBER, BOOLEAN, OBJECT, ARRAY };

  class iterator {
    friend class Object;

  public:
    iterator& operator++();
    iterator operator++(int);
    Object &operator*();
    [[nodiscard]] std::string first() const;
    Object &second();
    bool operator==(const iterator &rhs) const;
    bool operator!=(const iterator &rhs) const;

  private:
    explicit iterator(Object &ref);
    iterator(Object &ref, unsigned index);

    Object &m_ref;
    unsigned m_index;
  };

  class const_iterator {
    friend class Object;

  public:
    const_iterator& operator++();
    const_iterator operator++(int);
    Object operator*() const;
    [[nodiscard]] std::string first() const;
    [[nodiscard]] Object second() const;
    bool operator==(const const_iterator &rhs) const;
    bool operator!=(const const_iterator &rhs) const;

  private:
    explicit const_iterator(Object const &ref);
    const_iterator(Object const &ref, unsigned index);

    Object const &m_ref;
    unsigned m_index;
  };

  Object() = default;
  Object(Object const &other) = default;
  Object(Object &&other) noexcept ;
  explicit Object(std::string const &value);
  explicit Object(const char *value);
  explicit Object(bool const &value);
  explicit Object(short const &value);
  explicit Object(int const &value);
  explicit Object(float const &value);
  explicit Object(double const &value);
  explicit Object(std::vector<Object> const &value);
  explicit Object(std::nullptr_t);
  ~Object() = default;

  Object &operator=(Object const &other) = default;
  Object &operator=(Object &&other) noexcept ;
  Object &operator=(std::string const &value);
  Object &operator=(const char *value);
  Object &operator=(bool const &value);
  Object &operator=(short const &value);
  Object &operator=(int const &value);
  Object &operator=(float const &value);
  Object &operator=(double const &value);
  Object &operator=(std::vector<Object> const &value);
  Object &operator=(std::nullptr_t);

  bool operator==(const Object &rhs) const;
  bool operator!=(const Object &rhs) const;

  Object operator[](std::string const &key) const;
  Object &operator[](std::string const &key);
  Object operator[](size_t const &index) const;
  Object &operator[](size_t const &index);

  [[nodiscard]] Object::const_iterator begin() const;
  Object::iterator begin();
  [[nodiscard]] Object::const_iterator end() const;
  Object::iterator end();

  [[nodiscard]] Type type() const;
  [[nodiscard]] bool isObject() const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool isString() const;
  [[nodiscard]] bool isNumber() const;
  [[nodiscard]] bool isBoolean() const;
  [[nodiscard]] bool isNull() const;

  [[nodiscard]] size_t count() const;
  [[nodiscard]] std::vector<std::string> keys() const;
  [[nodiscard]] std::string key(unsigned index) const;
  [[nodiscard]] std::vector<int> sequence() const;
  [[nodiscard]] bool exists(std::string const &key) const;
  void add(std::string const &key, Object const &obj);
  void add(Object const &obj);
  void remove(std::string const &key);
  void remove(int const &index);
  void clear();

  [[nodiscard]] std::string toString() const;
  [[nodiscard]] bool toBoolean() const;
  [[nodiscard]] double toNumber() const;
  [[nodiscard]] std::vector<Object> toArray() const;

private:
  std::any m_value{jobject()};
  Type m_type{Type::OBJECT};
};

} // namespace jsonx
