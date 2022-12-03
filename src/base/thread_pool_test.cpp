/* Proj: cpp20-advanced-programming
 * File: thread_pool_test.cpp
 * Created Date: 2022/11/29
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/29 23:40:13
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include <gtest/gtest.h>
#include <helper/thread_pool.hpp>

TEST(THREAD_POOL, BIND_AND_LAMBDA) {
  ThreadPool tp;
  int32_t excepted_value = 0;
  const auto num_tasks = std::thread::hardware_concurrency() * 100;

  std::vector<std::future<int>> tasks;
  for (int i = 0; i < num_tasks; i++) {
    tasks.push_back(tp.execute(
        [&](int v) {
          if (v == num_tasks - 10) {
            std::this_thread::sleep_for(3s);
            printf("mock io task.\n");
          }
          return v * 20;
        },
        i));
    excepted_value += i * 20;
  }

  std::decay_t<decltype(excepted_value)> result = 0;
  for (auto& task : tasks) {
    result += task.get();
  }

  EXPECT_EQ(excepted_value, result);
}
