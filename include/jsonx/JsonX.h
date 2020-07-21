#pragma once

#include <jsonx/JsonObject.h>

#include <optional>

std::optional<jsonx::Object> operator "" _jsonx(const char* buffer, long unsigned int size);

namespace jsonx {

std::string stringify(Object const &obj);
void write(Object const &obj, std::string const path);
std::optional<Object> objectify(std::string const& buffer);
std::optional<Object> read(std::string const path);

} // namespace jsonx