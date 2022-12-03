/* Proj: cpp20-advanced-programming
 * File: future_join.cpp
 * Created Date: 2022/11/26
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/26 22:34:04
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <array>
#include <functional>
#include <future>
#include <vector>
#include "helper/common.hpp"

enum class Status {
  Success = 0,
};

template <typename T>
std::vector<T> join(std::vector<std::future<T>>& futures) noexcept {
  std::vector<T> results(futures.size());
  for (int i = 0; i < futures.size(); ++i) {
    std::cout << "i = " << i << std::endl;
    results[i] = futures[i].get();
  }
  return results;
}

template <typename Rep, typename Period>
auto timeout_f(std::chrono::duration<Rep, Period> delay) noexcept {
  std::this_thread::sleep_for(delay);
  return delay.count();
}

int64_t f() {
  return 0;
}

void test() {
  std::vector<std::future<int64_t>> wait_events;
  std::vector<std::packaged_task<int64_t()>> wait_tasks;

  for (int i = 0; i < 10; ++i) {
    std::packaged_task<int64_t()> task([] { return f(); });
    wait_events.push_back(task.get_future());
    wait_tasks.push_back(std::move(task));
  }

  auto res = join(wait_events);
  for (const auto& item : res) {
    std::cout << item << std::endl;
  }
}

int main() {
  test();
  return 0;
}
