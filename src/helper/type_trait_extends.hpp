/* Proj: cpp20-advanced-programming
 * File: type_trait_extends.hpp
 * Created Date: 2022/11/26
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/26 15:56:58
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_TYPE_TRAIT_EXTENDS_HPP
#define CPP20_ADVANCED_PROGRAMMING_TYPE_TRAIT_EXTENDS_HPP

#include <type_traits>

namespace detail {
template <typename T, typename Void = void>
struct has_value_type {
  constexpr static inline bool value = false;
};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> {
  constexpr static inline bool value = true;
};

template <typename T, std::enable_if_t<has_value_type<T>::value>* = nullptr>
struct get_value_type {
  using type = std::decay_t<typename T::value_type>;
};

} // namespace detail

template <typename T>
constexpr static inline bool is_always_false_v = false;

// template <typename T>
// constexpr static inline bool is_always_true_v = !is_always_false_v<T>;

template <typename T>
constexpr static inline bool has_value_type_v = detail::has_value_type<T>::value;

template <typename T>
using get_value_type_t = typename detail::get_value_type<T>::type;

//
struct Success {
  using value_type = int;
};

struct Failure {};

static_assert(has_value_type_v<Success>);
static_assert(!has_value_type_v<Failure>);

#endif // CPP20_ADVANCED_PROGRAMMING_TYPE_TRAIT_EXTENDS_HPP
