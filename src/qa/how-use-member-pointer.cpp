/* Proj: cpp20-advanced-programming
 * File: how-use-member-pointer.cpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 09:29:30
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <string>
#include <type_traits>

template <class Class, typename T>
void member_pointer(typename T::Class* p) {}

struct Foo {
  std::string data;
  void print() const noexcept { std::cout << "Foo: " << data << std::endl; }
};

void test() {
  Foo foo{.data = "foo"};
  void (Foo::*p)() const = &Foo::print;
  std::string Foo::*pData = &Foo::data;
  (foo.*p)();
  (foo.*pData) = "Hello World";
  (foo.Foo::print)();
}

int main() {
  test();
  return 0;
}
