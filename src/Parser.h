#pragma once

#include <jsonx/JsonObject.h>
#include <string>
#include <optional>

namespace jsonx {

class ObjectParser {
public:
  std::string operator()(Object const &obj) const;

private:
  static std::string parse(Object const &obj);
  static std::string fromObject(Object const &obj);
  static std::string fromArray(Object const &obj);
  static std::string fromNumber(Object const &obj);
  static std::string fromBoolean(Object const &obj);
  static std::string fromString(Object const &obj);
  static std::string fromNull(Object const &obj);
};

class StringParser {
  
  class StringIterator
  {
      public:
      StringIterator(std::string const& s);
      ~StringIterator() = default;

      char operator*() const;

      bool hasNext() const;
      bool next();
      void skipSpace();

      private:
      std::string::const_iterator m_iter;
      std::string::const_iterator m_end;
  };

public:
  std::optional<Object> operator()(std::string const &buffer) const;

private:
  static std::optional<Object> parse(StringIterator &iter);
  static std::optional<Object> toObject(StringIterator &iter);
  static std::optional<Object> toArray(StringIterator &iter);
  static std::optional<Object> toNumber(StringIterator &iter);
  static std::optional<Object> toBoolean(StringIterator &iter);
  static std::optional<Object> toString(StringIterator &iter);
  static std::optional<Object> toNull(StringIterator &iter);
};

} // namespace jsonx