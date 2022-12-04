/* Proj: cpp20-advanced-programming
 * File: excepted_test.cpp
 * Created Date: 2022/12/4
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/4 01:18:58
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include "./excepted.hpp"
#include <iostream>
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

TEST(EXCEPTION, COPY_AND_MOVE) {
  using ExceptedType = Excepted<MoveOnly, std::string>;

  auto te = ExceptedType::success(MoveOnly{"src object"});
  auto te2 = ExceptedType::failure("Some Error.");
  auto te3 = std::exchange(te2, std::move(te));

  std::cerr << te3.error() << std::endl;

  EXPECT_FALSE(te3);
  EXPECT_EQ(te2->data_, "src object");
}
