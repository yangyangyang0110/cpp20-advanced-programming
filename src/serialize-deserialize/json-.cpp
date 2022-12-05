/* Proj: cpp20-advanced-programming
 * File: json-.cpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 07:58:33
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <nlohmann/json.hpp>

#include "helper/excepted.hpp"

#include "helper/type_trait_extends.hpp"

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
constexpr static inline Property<Class, T> makeProperty(T Class::*p, char const* name) {
  return Property{p, name};
}

#define MAKE_PROPERTIES(...) const static inline std::tuple properties = __VA_ARGS__

template <typename T, T... Idx, typename F>
void for_sequence(std::integer_sequence<T, Idx...>, F&& f) {
  ((f(std::integral_constant<T, Idx>{})), ...);
}

template <class Class, std::enable_if_t<has_properties_type_v<Class>, int> = 0>
void fromJson(const json& data, Class& obj) noexcept {
  if (data.is_null())
    return;
  constexpr size_t num_properties = std::tuple_size_v<decltype(Class::properties)>;
  for_sequence(std::make_index_sequence<num_properties>{}, [&](auto item) {
    auto& property = std::get<item>(Class::properties);
    data.at(property.name).get_to(obj.*property.member);
  });
}

template <class Class, std::enable_if_t<has_properties_type_v<Class>, int> = 0>
void toJson(json& data, const Class& obj) noexcept {
  constexpr size_t num_properties = std::tuple_size_v<decltype(Class::properties)>;
  for_sequence(std::make_index_sequence<num_properties>{}, [&](auto item) {
    auto& property = std::get<item>(Class::properties);
    data[property.name] = obj.*property.member;
  });
}

#define TYPE_INTRUSIVE(type)                                    \
  friend void from_json(const json& data, type& obj) noexcept { \
    fromJson(data, obj);                                        \
  }                                                             \
  friend void to_json(json& data, const type& obj) noexcept {   \
    toJson(data, obj);                                          \
  }

#define SERIALIZE_ENUM(ENUM_TYPE, ...)                                             \
  template <typename BasicJsonType>                                                \
  inline void to_json(BasicJsonType& j, const ENUM_TYPE& e) {                      \
    static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!"); \
    static const std::pair<ENUM_TYPE, BasicJsonType> m[] = __VA_ARGS__;            \
    auto it = std::find_if(                                                        \
        std::begin(m),                                                             \
        std::end(m),                                                               \
        [e](const std::pair<ENUM_TYPE, BasicJsonType>& ej_pair) -> bool {          \
          return ej_pair.first == e;                                               \
        });                                                                        \
    j = ((it != std::end(m)) ? it : std::begin(m))->second;                        \
  }                                                                                \
  template <typename BasicJsonType>                                                \
  inline void from_json(const BasicJsonType& j, ENUM_TYPE& e) {                    \
    static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!"); \
    static const std::pair<ENUM_TYPE, BasicJsonType> m[] = __VA_ARGS__;            \
    auto it = std::find_if(                                                        \
        std::begin(m),                                                             \
        std::end(m),                                                               \
        [&j](const std::pair<ENUM_TYPE, BasicJsonType>& ej_pair) -> bool {         \
          return ej_pair.second == j;                                              \
        });                                                                        \
    e = ((it != std::end(m)) ? it : std::begin(m))->first;                         \
  }

struct Friend {
  std::string name;
  std::string email;

  friend std::ostream& operator<<(std::ostream& os, Friend const& rhs) noexcept {
    return os << "[Friend]: " << rhs.name << " " << rhs.email;
  }

  MAKE_PROPERTIES({
      makeProperty(&Friend::name, "f-username"),
      makeProperty(&Friend::email, "f-email"),
  });
  TYPE_INTRUSIVE(Friend);
};

enum TaskState {
  TS_STOPPED,
  TS_RUNNING,
  TS_COMPLETED,
  TS_INVALID = -1,
};

SERIALIZE_ENUM(
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
    os << "[User]: " << rhs.uname << ", " << rhs.passwd << ", " << std::boolalpha
       << rhs.flag << std::noboolalpha
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

  TYPE_INTRUSIVE(User);
};

void test() {
  json data = R"({
    "username": "I'm username",
    "password": "I'm password",
    "is_used": 1,
    "state": "running",
    "fs": [
      {"f-username": "f-username1", "f-email": "f-email1"},
      {"f-username": "f-username2", "f-email": "f-email2"}
    ],
    "fm": {
      "f1": {
        "f-username": "I'm username",
        "f-email": "I'm email1"
      },
      "f2": {
        "f-username": "I'm username",
        "f-email": "I'm email2"
      }
    }
  })"_json;

  auto user = data.get<User>();
  std::cout << user << std::endl;

  user.taskState = TaskState::TS_STOPPED;
  json deserialize_data(user);
  std::cout << deserialize_data << std::endl;
  std::cout << std::boolalpha << (deserialize_data == data) << std::noboolalpha
            << std::endl;
}

int main() {
  test();
  return 0;
}
