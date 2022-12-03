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
#include "factory.hpp"
#include "strategy/mod.hpp"

#define CREATE_V(type, ...) Factory::create<type>(#type, __VA_ARGS__)

#define COUNT_ARGS(...) \
  std::cout << std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))> << std::endl

struct Foo {
  Foo(int a, int b) noexcept : a(a), b(b) {}

  friend std::ostream& operator<<(std::ostream& os, const Foo& foo) noexcept {
    return os << "a: " << foo.a << " b: " << foo.b;
  }

private:
  int a{}, b{};
};

int main() {
  // Foo foo(10, 20);
  // std::cout << foo << std::endl;

  Factory::registry("Foo", (void*)std::make_shared<Foo, int, int>);

  auto f = Factory::getObject<Foo>("Foo", 30, 20);

  std::cout << *f << std::endl;

  return 0;
}
