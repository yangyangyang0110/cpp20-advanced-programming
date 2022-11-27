/* Proj: cpp20-advanced-programming
 * File: demo.cpp
 * Created Date: 2022/11/26
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/26 10:01:49
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <functional>
#include <iostream>
#include <optional>
#include "helper/common.hpp"
#include "helper/type_trait_extends.hpp"

#include "simple_future.hpp"

/**
 * @brief 例子: socket数据读取, 数据是否准备好通过optional语义去表示
 * @tparam T
 */

struct Socket : public NonCopyable {
public:
  using value_type = std::string;
  using time_point = std::chrono::steady_clock;

public:
  explicit Socket(value_type data) noexcept
      : start_time_(time_point::now()), buffer_data_(std::move(data)) {}

  template <typename Rep, typename Period>
  [[nodiscard]] bool has_data_to_read(
      const std::chrono::duration<Rep, Period>& delay) const noexcept {
    return time_point::now() - start_time_ >= delay;
  }

  [[nodiscard]] value_type read_buf() const noexcept { return buffer_data_; }

  void set_wakeup_callback(const std::function<void()>& wakeup_callback) noexcept {}

private:
  value_type buffer_data_;
  time_point::time_point start_time_;
};

template <>
struct SimpleFuture<Socket> {
  using value_type = get_value_type_t<Socket>;
  static std::optional<value_type> poll(
      Socket& socket, const WakeupFunction& wake) noexcept {
    if (socket.has_data_to_read(2s)) {
      // The socket has data -- read it into a buffer and return it.
      return socket.read_buf();
    } else {
      // The socket does not yet have data.
      //
      // Arrange for `wake` to be called once data is available.
      // When data becomes available, `wake` will be called, and the
      // user of this `Future` will know to call `poll` again and
      // receive data.
      socket.set_wakeup_callback(wake);
      return std::nullopt;
    }
  }
};

void test() {
  // Mock coroutine scheduler?
  Socket socket("Some socket data.");
  LOG_PRINT("Starting...");
  while (true) {
    auto opt = SimpleFuture<Socket>::poll(
        socket, []() { std::cout << "Callback called" << std::endl; });
    if (opt) {
      LOG_PRINT("get data: ", *opt);
      break;
    }
  }
  LOG_PRINT("Ended.");
}

int main() {
  test();
  return 0;
}
