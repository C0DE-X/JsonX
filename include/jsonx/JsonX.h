#pragma once

#include <jsonx/JsonObject.h>

namespace jsonx {

class Stringify {
public:
  std::string operator()(Object const &obj) const;

private:
  std::string toString(Object const &obj) const;
  std::string fromObject(Object const &obj) const;
  // void fromArray(Object const& obj, std::string& buffer) const;
  std::string fromNumber(Object const &obj) const;
  std::string fromBoolean(Object const &obj) const;
  std::string fromString(Object const &obj) const;
};

class Objectify {
public:
  Object operator()(std::string const &buffer) const;

private:
  Object fromString(std::string& buffer) const;
  Object toObject(std::string& buffer) const;
  Object toNumber(std::string& buffer) const;
  Object toBoolean(std::string& buffer) const;
  Object toString(std::string& buffer) const;

  void skipWhiteSpaces(std::string& buffer) const;
};

class Writer {
public:
  void operator()(Object const &obj, std::string const path) const;
};

} // namespace jsonx