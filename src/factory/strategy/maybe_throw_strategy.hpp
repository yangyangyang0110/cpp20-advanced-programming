/* Proj: cpp20-advanced-programming
 * File: maybe_throw_strategy.hpp
 * Created Date: 2022/12/4
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/4 20:15:36
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_MAYBE_THROW_STRATEGY_HPP
#define CPP20_ADVANCED_PROGRAMMING_MAYBE_THROW_STRATEGY_HPP

#include <stdexcept>
struct MaybeThrow {
  explicit MaybeThrow(bool is_throw) {
    if (is_throw) {
      throw std::runtime_error("Throwing...");
    }
  }
};

#endif // CPP20_ADVANCED_PROGRAMMING_MAYBE_THROW_STRATEGY_HPP
