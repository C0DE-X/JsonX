#pragma once

#include <jsonx/JsonObject.h>

#include <optional>

std::optional<jsonx::Object> operator "" _jsonx(const char* buffer, long unsigned int size);
std::ostream& operator<<(std::ostream& os, const jsonx::Object& obj);

namespace jsonx {

constexpr std::nullptr_t null { nullptr };
static Object Array() { return Object(std::vector<Object>()); }

std::string stringify(Object const &obj);
void write(Object const &obj, std::string const& path);
std::optional<Object> objectify(std::string const& buffer);
std::optional<Object> read(std::string const& path);

} // namespace jsonx