/* Proj: cpp20-advanced-programming
 * File: socket_future.cpp
 * Created Date: 2022/11/27
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/27 18:59:36
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include "socket_future.hpp"

int main() {
  // Mock coroutine scheduler?
  Socket socket("Some socket data.");
  LOG_PRINT("Starting...");
  while (true) {
    if (auto opt = SimpleFuture<Socket>::poll(
            socket, []() { std::cout << "Callback called" << std::endl; })) {
      LOG_PRINT("get data: ", *opt);
      break;
    }
  }
  LOG_PRINT("Ended.");

  return 0;
}
