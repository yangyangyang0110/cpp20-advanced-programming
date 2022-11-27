/* Proj: cpp20-advanced-programming
 * File: ad_hoc.cpp
 * Created Date: 2022/11/27
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/27 16:25:01
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>

struct Foo {};

struct Bad {};

template <typename T>
struct Base {
  using value_type = void;
};

template <>
struct Base<Foo> {
  using value_type = Foo;
};

void test() {
  // dispatch.
  static_assert(std::is_same_v<Base<Bad>::value_type, void>);
  static_assert(std::is_same_v<Base<Foo>::value_type, Foo>);
}

int main() {
  test();
  return 0;
}
