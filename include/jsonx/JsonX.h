#pragma once

#include <jsonx/JsonObject.h>

#include <optional>

std::optional<jsonx::Object> operator "" _jsonx(const char* buffer, long unsigned int size);
std::ostream& operator<<(std::ostream& os, const jsonx::Object& obj);

namespace jsonx {

constexpr nullptr_t null { nullptr };
const Object Array();

std::string stringify(Object const &obj);
void write(Object const &obj, std::string const path);
std::optional<Object> objectify(std::string const& buffer);
std::optional<Object> read(std::string const path);

} // namespace jsonx