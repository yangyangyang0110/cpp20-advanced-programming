/* Proj: Project
 * File: how_fix_bind_forward_move_only.cpp
 * Created Date: 2022/12/3
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/3 11:34:39
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <functional>
#include <future>
#include <iostream>

template <typename F, typename... Args, typename R = std::invoke_result_t<F, Args...>>
std::packaged_task<R()> wrap(F&& f, Args&&... args) {
  return std::packaged_task<R()>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}

struct MoveOnly {
  std::string data_;
  int32_t move_constructor_ = 0, move_assignment_ = 0;

  explicit MoveOnly(std::string data) noexcept : data_(std::move(data)) {}

  MoveOnly(const MoveOnly& other) = delete;
  MoveOnly& operator=(const MoveOnly& other) = delete;

  MoveOnly(MoveOnly&& other) noexcept : data_(std::move(other.data_)) {
    ++move_constructor_;
  }

  MoveOnly& operator=(MoveOnly&& other) noexcept {
    if (this != &other)
      data_ = std::move(other.data_);
    ++move_assignment_;
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const MoveOnly& ins) noexcept {
    return std::cout << "data: " << ins.data_;
  }
};

template <typename F, typename... Args, typename R = std::invoke_result_t<F, Args...>>
R mBind(F&& f, Args&&... args) {
  return std::apply(std::forward<F>(f), std::make_tuple(std::forward<Args>(args)...));
}

void f(MoveOnly&& value) {}

struct Foo {
  std::string show(std::string&& value) {
    value.append("<<<(Foo)");
    // std::cout << value << std::endl;
    return value;
  }
};

// template <typename F, typename... Args>
// auto m_bind(F&& f, Args&&... args) noexcept {
//   return FArgs{}(std::forward<F>(f), std::forward<Args>(args)...);
// }

void test() noexcept {
  std::string value = "hello world";
  // m_bind(f, std::move(value));
  MoveOnly moveOnly{"move"};

  mBind(f, std::move(moveOnly));

  value = "test for member function";
  auto res = mBind(&Foo::show, Foo{}, std::move(value));
  std::cout << res << std::endl;
}

int main() {
  test();
  return 0;
}
