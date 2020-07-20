#pragma once

#include <string>
#include <jsonx/JsonObject.h>

namespace jsonx
{

class Parser
{
public:
  std::string operator()(Object const &obj) const;

private:
  static std::string parse(Object const &obj);
  static std::string fromObject(Object const &obj);
  static std::string fromArray(Object const& obj);
  static std::string fromNumber(Object const &obj);
  static std::string fromBoolean(Object const &obj);
  static std::string fromString(Object const &obj);
};


}