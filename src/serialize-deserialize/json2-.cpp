/* Proj: cpp20-advanced-programming
 * File: json2-.cpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 09:20:52
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include <iostream>
#include <nlohmann/json.hpp>

#include "helper/excepted.hpp"

using nlohmann::json;
using namespace nlohmann::literals;

template <class Class, typename T>
struct Property {
  using type = T;
  Property(T Class::*aMemberPointer, char const* aMemberName) noexcept
      : member(aMemberPointer), name(aMemberName) {}
  T Class::*member;
  char const* name;
};

template <class Class, typename T>
inline Property<Class, T> makeProperty(T Class::*p, char const* name) {
  return Property{p, name};
}

static_assert(std::is_same_v<char const*, const char*>);
static_assert(!std::is_same_v<char* const, const char*>);

bool int2bool(int v) noexcept {
  return v > 0;
}

char const* bool2cstr(bool b) noexcept {
  return b ? "true" : "false";
}

#define MAKE_PROPERTIES(...) \
  const static inline std::tuple properties = __VA_ARGS__

struct User {
  int flag{};
  std::string uname;
  std::string passwd;

  friend std::ostream& operator<<(std::ostream& os, const User& rhs) noexcept {
    return os << "[User]: " << rhs.uname << ", " << rhs.passwd << ", "
              << bool2cstr(int2bool(rhs.flag));
  }

  MAKE_PROPERTIES({
      makeProperty(&User::uname, "username"),
      makeProperty(&User::passwd, "password"),
      makeProperty(&User::flag, "is_used"),
  });
};

template <class T>
concept HasPropertiesType = requires { T::properties; };

template <typename T, T... Idx, typename F>
constexpr void for_sequence(std::integer_sequence<T, Idx...>, F&& f) {
  ((f(std::integral_constant<T, Idx>{})), ...);
}

// template <typename T>
template <HasPropertiesType T>
T from_json(const json& data) noexcept {
  if (data.is_null())
    return {};

  T res;
  // get properties size;
  constexpr size_t num_properties = std::tuple_size_v<decltype(T::properties)>;

  // loop at compile time.
  for_sequence(std::make_index_sequence<num_properties>{}, [&](auto i) {
    // get property.
    auto& property = std::get<i>(T::properties);
    using Type = typename std::decay_t<decltype(property)>::type;
    // data.at(property.name).get_to(res.*property.member);
    res.*property.member = data[property.name].template get<Type>();
  });

  return res;
}

void test() {
  json data = R"({
    "username": "I'm username",
    "password": "I'm password",
    "is_used": 1
  })"_json;

  HasPropertiesType auto user = from_json<User>(data);
  // auto user = from_json<User>(data);
  std::cout << user << std::endl;
}

int main() {
  test();
  return 0;
}
