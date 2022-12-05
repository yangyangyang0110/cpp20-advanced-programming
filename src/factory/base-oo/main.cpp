/* Proj: cpp20-advanced-programming
 * File: main.cpp
 * Created Date: 2022/11/17
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/17 23:14:15
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
// #include "./strategy/base.hpp"
#include "./strategy/mod.hpp"
#include "factory.hpp"

#define COUNT_ARGS(...) \
  std::cout << std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))> << std::endl

struct A {
  A() noexcept { std::cout << "A::A()" << std::endl; }

  virtual void Update() noexcept { std::cout << "A::Update()" << std::endl; }

  friend std::ostream& operator<<(std::ostream& os, const A&) noexcept {
    return os << "A";
  }
};

struct B : public A {
  B() noexcept { std::cout << "B::B()" << std::endl; }

  void Update() noexcept override { std::cout << "B::Update()" << std::endl; }

  friend std::ostream& operator<<(std::ostream& os, const B&) noexcept {
    return os << "B";
  }
};

int main() {
  /*
    CallPhone: minimum area: 2 desc: desc
    Update: 334313328(随机), 发生了内存不安全的行为
   */

  if (auto exc = Factory::getObject<Base>("CallPhone", CallPhoneParam{"desc", 2})) {
    CallPhoneFrameData data{};
    exc.value()->Update(&data);
    // exc->Update();
  } else {
    std::cerr << "Failure: " << exc.error() << std::endl;
  }

  // 类型反擦除错误, 程序有概率crash
  if (auto exc = Factory::getObject<Base>("CallSmoke", CallPhoneParam{"desc", 2})) {
    CallSmokeFrameData data{};
    exc.value()->Update(&data);
    // exc->Update();
  } else {
    std::cerr << "Failure: " << exc.error() << std::endl;
  }

  // if (auto exc = Factory::getObject("MaybeThrow", false); !exc) {
  //   std::cerr << exc.error() << std::endl;
  // }

  return 0;
}
