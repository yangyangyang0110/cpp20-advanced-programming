/*
 * File: timer.hpp
 * Project: helper
 * File Created: 2022-08-28 13:55:12
 * Author: yangyangyang (yangyangyang0110@outlook.com)
 * -----
 * Last Modified: 2022-08-28 13:55:19
 * Modified By: yangyangyang (yangyangyang0110@outlook.com>)
 * -----
 * Copyright 2022 - 2022  ,  
 */

#pragma once

#include <chrono>

#define TIMER_START(name) helper::timer::Timer timer_##name(true)
#define TIMER_END(name)                                                        \
  printf("[%s] time cost %lu(ms).\n", #name, timer_##name.GetTimeCost())


#define TIMER_START_RENDER(name) helper::timer::Timer timer_name(true)
#define TIMER_END_RENDER(name)                                                 \
  printf("[%s] time cost %lu(ms).\n", name, timer_name.GetTimeCost())

#define TIMER_NS_START_RENDER(name) helper::timer::Timer timer_ns_name(true)
#define TIMER_NS_END_RENDER(name)                                              \
  printf("[%s] time cost %lu(ns).\n", name,                                    \
         timer_ns_name.GetTimeCost<std::chrono::nanoseconds>())

namespace helper::timer {

struct Timer {

  explicit Timer(bool reset) {
    if (reset)
      Reset();
  }

  void Reset() { now_ = std::chrono::system_clock::now(); }

  template<typename T = std::chrono::milliseconds>
  T GetTimeCost() const noexcept {
    return std::chrono::duration_cast<T>(std::chrono::system_clock::now() -
                                         now_);
  }

private:
  std::chrono::system_clock::time_point now_;
};

} // namespace helper::timer
