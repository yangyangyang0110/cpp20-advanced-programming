/* Proj: cpp20-advanced-programming
 * File: common.hpp
 * Created Date: 2022/11/26
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/26 15:57:28
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_COMMON_HPP
#define CPP20_ADVANCED_PROGRAMMING_COMMON_HPP

#include <chrono>
#include <iomanip>
#include <iostream>

static inline std::time_t get_time_t() noexcept {
  return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

namespace detail {

template <typename Tuple, std::size_t... Index>
static void LOG_PRINT_IMPL(Tuple&& tuple, std::index_sequence<Index...>) noexcept {
  ((std::cout << (Index != 0 ? ", " : "") << std::get<Index>(tuple)), ...);
}

} // namespace detail

template <typename... Outputs>
static void LOG_PRINT(Outputs&&... outputs) noexcept {
  auto time = get_time_t();
  std::cout << "[" << std::put_time(std::localtime(&time), "%F %T") << "] ";
  detail::LOG_PRINT_IMPL(
      std::make_tuple(std::forward<Outputs>(outputs)...),
      std::index_sequence_for<Outputs...>{});
  std::cout << std::endl;
}

using namespace std::chrono_literals;

struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif // CPP20_ADVANCED_PROGRAMMING_COMMON_HPP
