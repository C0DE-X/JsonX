#pragma once

#include <jsonx/JsonObject.h>

#include <optional>

namespace jsonx {

std::string stringify(Object const &obj);
void write(Object const &obj, std::string const path);
std::optional<Object> objectify(std::string const& buffer);

} // namespace jsonx