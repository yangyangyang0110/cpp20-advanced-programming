/* Proj: cpp20-advanced-programming
 * File: test-logger.cpp
 * Created Date: 2022/10/19
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/19 12:13:25
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <filesystem>
#include "helper/logger.hpp"

namespace fs = std::filesystem;

void testSingleThread() {
  std::cout << "current_path: " << fs::current_path() << std::endl;
  helper::logger::initializeGlobalLogger<helper::logger::FileSizeType::MB>(
      helper::logger::LogLevel::DEBUG,
      fs::current_path() / fs::path("logs"),
      "test.log",
      1,
      4);

  for (int i = 0; i < 10; ++i) {
    LOG_TRACE("i = {}", i);
    LOG_DEBUG("i = {}", i);
    LOG_INFO("i = {}", i);
    LOG_WARN("i = {}", i);
    LOG_ERROR("i = {}", i);
  }
}

void test() {
  testSingleThread();
  // testMultiThread();
}

int main() {
  test();
  return 0;
}
