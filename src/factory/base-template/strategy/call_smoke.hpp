/* Proj: cpp20-advanced-programming
 * File: call_smoke.hpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 13:57:51
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_CALL_SMOKE_HPP
#define CPP20_ADVANCED_PROGRAMMING_CALL_SMOKE_HPP

#include <iostream>

struct CallSmokeFrameData {};

class CallSmoke {
public:
  explicit CallSmoke(int value) noexcept : value_(value) {}

  void update(const CallSmokeFrameData& param) const noexcept {
    std::cout << "update: " << value_ << std::endl;
  }

private:
  int value_;
};

#endif // CPP20_ADVANCED_PROGRAMMING_CALL_SMOKE_HPP
