/* Proj: Project
 * File: test.cpp
 * Created Date: 2022/12/2
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/2 21:55:57
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <variant>

#define ASSERT(condition, msg)                                             \
  do {                                                                     \
    if (!(condition)) {                                                    \
      std::cerr << "Assert: " << #condition << " && " << msg << std::endl; \
      abort();                                                             \
    }                                                                      \
  } while (0)

namespace detail {

template <class T, typename Void = void>
struct has_param_type : std::bool_constant<false> {};

template <class T>
struct has_param_type<T, std::void_t<typename T::param_type>> : std::bool_constant<true> {
};

template <class T, std::enable_if_t<has_param_type<T>::value, int> = 0>
struct get_param_type {
  using type = typename T::param_type;
};

} // namespace detail

template <class T>
constexpr static inline bool has_param_type_v = detail::has_param_type<T>::value;

template <class T>
using get_param_type_t = typename detail::get_param_type<T>::type;

/**
 *
 */

enum AlgorType : int32_t { UNKNOWN = -1, FACE = 0, PERSON };

struct FaceParam {
  int x;
  float y;
  double z;

  friend std::ostream& operator<<(std::ostream& os, const FaceParam& ins) noexcept {
    return os << "[FaceParam] " << ins.x << ", " << ins.y << ", " << ins.z;
  }
};

struct PersonParam {
  std::string data;

  friend std::ostream& operator<<(std::ostream& os, const PersonParam& ins) noexcept {
    return os << "[PersonParam] " << ins.data;
  }
};

struct ParamWrap {
  int algorType;
  void* param;
};

//! int to param_type mapping.
template <int>
struct X {};

#define REGISTRY_MAP(algorType, paramType) \
  template <>                              \
  struct X<algorType> {                    \
    using param_type = paramType;          \
  }

REGISTRY_MAP(FACE, FaceParam);
REGISTRY_MAP(PERSON, PersonParam);

#define TMP                       1 << 2

#define SENSOR_CONFIG0            0
#define SENSOR_CONFIG0_STOP       BIT(0)
#define SENSOR_CONFIG0_CPTR_OVER  BIT(2)
#define SENSOR_CONFIG0_OVER       BIT(3)
#define SENSOR_CONFIG0_TCALC_OVER BIT(4)
#define SENSOR_CONFIG0_TALL_MASK  (0xfffff << 8)
#define SENSOR_CONFIG0_TALL_SHIFT 8

//! Assert
static_assert(has_param_type_v<X<FACE>>);
static_assert(!has_param_type_v<X<-1>>);

template <int e, typename R = get_param_type_t<X<e>>>
inline R toParamType(const void* p) noexcept {
  return *(R*)p; // Copy Param
}

std::variant<PersonParam, FaceParam> toParamType(const ParamWrap& wrap) noexcept {
  switch (wrap.algorType) {
    case FACE:
      return toParamType<FACE>(wrap.param);
    case PERSON:
      return toParamType<PERSON>(wrap.param);
    default:
      return {};
  }
}

class ConfigCenter {
public:
  using param_type = std::variant<PersonParam, FaceParam>;

  explicit ConfigCenter() noexcept = default;

  template <bool UpdateIf = true>
  void Add(const std::string& key, const ParamWrap& paramWrap) noexcept {
    if constexpr (UpdateIf) {
      paramMap_[key] = toParamType(paramWrap);
    } else {
      if (auto it = paramMap_.find(key); it == paramMap_.end()) {
        paramMap_[key] = toParamType(paramWrap);
      }
    }
  }

  template <class T>
  const T* Get(const std::string& key) noexcept {
    if (auto it = paramMap_.find(key); it != paramMap_.end()) {
      return std::get_if<T>(&it->second);
    }
    return nullptr;
  }

private:
  std::map<std::string, param_type> paramMap_;
};

void test() {
  ConfigCenter configCenter;

  {
    FaceParam faceParam{1, 2.0f, 3.0};
    PersonParam personParam{"person"};

    configCenter.Add("Face", ParamWrap{FACE, &faceParam});
    configCenter.Add("Person", ParamWrap{PERSON, &personParam});

    ASSERT(configCenter.Get<FaceParam>("Face"), "Type Error");
    ASSERT(configCenter.Get<PersonParam>("Person"), "Type Error");
    std::cout << *configCenter.Get<FaceParam>("Face") << std::endl;
    std::cout << *configCenter.Get<PersonParam>("Person") << std::endl;
  }

  {
    FaceParam faceParam{5, 20.0f, 30.0};
    PersonParam personParam{"xxx"};

    configCenter.Add("Face", ParamWrap{FACE, &faceParam});
    configCenter.Add("Person", ParamWrap{PERSON, &personParam});

    ASSERT(configCenter.Get<FaceParam>("Face"), "Type Error");
    ASSERT(configCenter.Get<PersonParam>("Person"), "Type Error");
    std::cout << *configCenter.Get<FaceParam>("Face") << std::endl;
    std::cout << *configCenter.Get<PersonParam>("Person") << std::endl;
  }

  {
    FaceParam faceParam{5, 20.0f, 30.0};
    PersonParam personParam{"yyy"};

    configCenter.Add("Face", ParamWrap{UNKNOWN, &faceParam});
    configCenter.Add("Person", ParamWrap{PERSON, &personParam});

    // ASSERT(configCenter.Get<FaceParam>("Face"), "Type Error");
    ASSERT(configCenter.Get<PersonParam>("Person"), "Type Error");
    // std::cout << *configCenter.Get<FaceParam>("Face") << std::endl;
    std::cout << *configCenter.Get<PersonParam>("Person") << std::endl;
  }
}

int main() {
  test();
  return 0;
}
