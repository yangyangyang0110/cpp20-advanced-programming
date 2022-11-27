/* Proj: cpp20-advanced-programming
 * File: socket_future_test.cpp
 * Created Date: 2022/11/27
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/27 19:01:06
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include "socket_future.hpp"
#include <gtest/gtest.h>

TEST(Future, BasicAssertions) {
  Socket::value_type input = "Some socket data.";
  Socket::value_type result;
  result.resize(input.size());

  Socket socket("Some socket data.");
  LOG_PRINT("Starting...");
  while (true) {
    if (auto opt = SimpleFuture<Socket>::poll(
            socket, []() { std::cout << "Callback called" << std::endl; })) {
      result = std::move(*opt);
      LOG_PRINT("get data: ", result);
      break;
    }
  }
  LOG_PRINT("Ended.");
  EXPECT_EQ(input, result);
}
