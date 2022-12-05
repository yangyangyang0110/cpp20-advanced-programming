/* Proj: cpp20-advanced-programming
 * File: thread_pool_test.cpp
 * Created Date: 2022/12/3
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/3 13:30:03
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include "./thread_pool.hpp"
#include <gtest/gtest.h>

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

auto f(MoveOnly&& value, const std::string& append_data) {
  value.data_.append(append_data);
  return value;
}

TEST(THREAD_POOL, FOR_FUNCTION) {
  if (auto ptr = ThreadPool::getWeakInstance().lock()) {
    MoveOnly v{"value for function"};
    std::stringstream ss;
    ss << v.data_ << "(f)";
    auto fut = ptr->execute(f, std::move(v), std::string{"(f)"});
    EXPECT_EQ(fut.get().data_, ss.str());
  }
  ThreadPool::release();
}

struct Foo {
  auto show(MoveOnly&& value, const std::string& append_value) {
    value.data_.append(append_value);
    return value;
  }
};

TEST(THREAD_POOL, FOR_MEMBER_FUNCTION) {
  if (auto ptr = ThreadPool::getWeakInstance().lock()) {
    MoveOnly v{"value for member function"};
    std::stringstream ss;
    ss << v.data_ << "(foo)";
    auto fut = ptr->execute(&Foo::show, Foo{}, std::move(v), std::string{"(foo)"});
    EXPECT_EQ(fut.get().data_, ss.str());
  }
  ThreadPool::release();
}

TEST(THREAD_POOL, FOR_STACK_VAR) {
  if (auto ptr = ThreadPool::getWeakInstance().lock()) {
    int x = 10;
    auto fut = ptr->execute([](int v) { return v; }, x); // Compiler error.
    // ptr->execute([](int v) { return v; }, int(x));
    EXPECT_EQ(fut.get(), x);
  }
}
