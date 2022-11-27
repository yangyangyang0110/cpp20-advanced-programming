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
#include <future>
#include <vector>
#include "helper/common.hpp"

template <typename T>
std::vector<T> join(std::vector<std::future<T>>& futures) noexcept {
  std::vector<T> results(futures.size());
  for (int i = 0; i < futures.size(); ++i) {
    results[i] = futures[i].get();
  }
  return results;
}

template <typename Rep, typename Period>
Rep timeout_f(std::chrono::duration<Rep, Period> delay) noexcept {
  std::this_thread::sleep_for(delay);
  return delay.count();
}

void test() {
  std::packaged_task<int()> task([]() { return 10; });
  std::future<int> future1 = task.get_future();

  std::packaged_task<int()> task2([]() { return 20; });
  std::future<int> future2 = task2.get_future();

  // join<int>({std::move(future1), std::move(future2)});
}

int main() {
  test();
  return 0;
}
