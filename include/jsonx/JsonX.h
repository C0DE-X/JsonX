#pragma once

#include <jsonx/JsonObject.h>

namespace jsonx {

class Stringify {
public:
  std::string operator()(Object const &obj) const;

private:
  void toString(Object const &obj, std::string &buffer) const;
  void fromObject(Object const &obj, std::string &buffer) const;
  // void fromArray(Object const& obj, std::string& buffer) const;
  void fromNumber(Object const &obj, std::string &buffer) const;
  void fromBoolean(Object const &obj, std::string &buffer) const;
  void fromString(Object const &obj, std::string &buffer) const;
};

class Objectify {
public:
  Object operator()(std::string const &buffer) const;
};

class Writer {
public:
  void operator()(Object const &obj, std::string const path) const;
};

} // namespace jsonx