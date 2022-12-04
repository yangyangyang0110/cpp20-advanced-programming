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

#define MAKE_PROPERTIES(...) const static inline std::tuple properties = __VA_ARGS__

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

bool int2bool(int v) noexcept {
  return v > 0;
}

char const* bool2cstr(bool b) noexcept {
  return b ? "true" : "false";
}

enum AlgorithmType { UNKNOWN = -1, PEDESTRIAN_DIRECTION_COUNTING, FACE_SNAPSHOT };

NLOHMANN_JSON_SERIALIZE_ENUM(
    AlgorithmType,
    {
        {UNKNOWN, nullptr},
        {PEDESTRIAN_DIRECTION_COUNTING, "PEDESTRIAN_DIRECTION_COUNTING"},
        {FACE_SNAPSHOT, "FACE_SNAPSHOT"},
    })

struct PedestrianDirectionCountingParams {};

struct FaceSnapshotParams {};

using Param = std::variant<PedestrianDirectionCountingParams, FaceSnapshotParams>;

struct Task {
  Param param;
  AlgorithmType algorithmType;
};

struct AddTask {
  std::string streamUrl;
  std::map<AlgorithmType, Param> params;
};

void test() {
  // Set data.
  json data = R"({
    "rtsp": "rtsp://localhost:8554/some-video",
    "params": {
      "FaceSnapshot": {},
    }
  })"_json;

  // Parse data.
  AddTask addTask;
}

int main() {
  test();
  return 0;
}
