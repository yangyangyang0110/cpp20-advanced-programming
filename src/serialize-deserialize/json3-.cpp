/* Proj: cpp20-advanced-programming
 * File: json3-.cpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 12:37:11
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

#define MAKE_PROPERTIES(...) \
  const static inline std::tuple properties = __VA_ARGS__

bool int2bool(int v) noexcept {
  return v > 0;
}

char const* bool2cstr(bool b) noexcept {
  return b ? "true" : "false";
}

struct Friend {
  std::string name;
  std::string email;

  friend std::ostream& operator<<(
      std::ostream& os, Friend const& rhs) noexcept {
    return os << "[Friend]: " << rhs.name << " " << rhs.email;
  }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Friend, name, email)

  const static inline std::tuple properties = {
      makeProperty(&Friend::name, "f-username"),
      makeProperty(&Friend::email, "f-email")};
};
enum TaskState {
  TS_STOPPED,
  TS_RUNNING,
  TS_COMPLETED,
  TS_INVALID = -1,
};

// map TaskState values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM(
    TaskState,
    {
        {TS_INVALID, nullptr},
        {TS_STOPPED, "stopped"},
        {TS_RUNNING, "running"},
        {TS_COMPLETED, "completed"},
    })

struct User {
  int flag{};
  std::string uname;
  std::string passwd;
  std::vector<Friend> friends;
  std::map<std::string, Friend> friendsMap;
  TaskState taskState{TS_INVALID};

  friend std::ostream& operator<<(std::ostream& os, const User& rhs) noexcept {
    os << "[User]: " << rhs.uname << ", " << rhs.passwd << ", "
       << bool2cstr(int2bool(rhs.flag))
       << " state: " << static_cast<int32_t>(rhs.taskState) << std::endl;

    for (const auto& f : rhs.friends) {
      os << "  " << f << std::endl;
    }

    for (auto&& [k, v] : rhs.friendsMap) {
      os << k << ": " << v << std::endl;
    }

    return os;
  }

  MAKE_PROPERTIES({
      makeProperty(&User::uname, "username"),
      makeProperty(&User::passwd, "password"),
      makeProperty(&User::flag, "is_used"),
      makeProperty(&User::friends, "fs"),
      makeProperty(&User::friendsMap, "fm"),
      makeProperty(&User::taskState, "state"),
  });
};

template <class T>
concept HasPropertiesType = requires { T::properties; };

template <typename T, T... Idx, typename F>
void for_sequence(std::integer_sequence<T, Idx...>, F&& f) {
  ((f(std::integral_constant<T, Idx>{})), ...);
}

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
    data.at(property.name).get_to(res.*property.member);
  });

  return res;
}

void test() {
  json data = R"({
    "username": "I'm username",
    "password": "I'm password",
    "is_used": 1,
    "state": "running",
    "fs": [
      {"name": "f-username1", "email": "f-email1"},
      {"name": "f-username2", "email": "f-email2"}
    ],
    "fm": {
      "f1": {
        "name": "I'm username",
        "email": "I'm email1"
      },
      "f2": {
        "name": "I'm username",
        "email": "I'm email2"
      }
    }
  })"_json;

  HasPropertiesType auto user = from_json<User>(data);
  std::cout << user << std::endl;
}

int main() {
  test();
  return 0;
}
