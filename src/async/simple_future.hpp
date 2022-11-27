/* Proj: cpp20-advanced-programming
 * File: simple_future.hpp
 * Created Date: 2022/11/26
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/26 22:10:16
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_SIMPLE_FUTURE_HPP
#define CPP20_ADVANCED_PROGRAMMING_SIMPLE_FUTURE_HPP

template <typename T>
concept FutureAble = requires { has_value_type_v<T>; };

using WakeupFunction = std::function<void()>;

// Define Interface.
template <FutureAble T>
struct SimpleFuture {
  using value_type = get_value_type_t<T>;
  std::optional<value_type> poll(const WakeupFunction& wake) noexcept {
    static_assert(is_always_false_v<value_type>, "This is Only Interface.");
  }
};

class EventLoop {};

#endif // CPP20_ADVANCED_PROGRAMMING_SIMPLE_FUTURE_HPP
