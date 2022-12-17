/* Proj: cpp20-advanced-programming
 * File: main.cpp
 * Created Date: 2022/12/4
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/4 12:09:38
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <tuple>
#include <vector>
#include "./util.hpp"

// #define MAKE_PROPERTIES(...) const static inline std::tuple properties = __VA_ARGS__

#define MAKE_METHOD(...) const static inline std::tuple methods = __VA_ARGS__

struct Foo {
  explicit Foo(int data) noexcept : base_(data){};
  [[nodiscard]] int f(int v) const noexcept {
    std::cout << "f is called" << std::endl;
    return base_ + v;
  };

  int base_;
  std::string name;

  // MAKE_PROPERTIES({reflect::makeProperty(&Foo::base_, "data")});
};

void test() {
  Foo foo{10};
  std::string Foo::*p = &Foo::name;

  // foo.*p;

  // auto ret = MemberFunction<decltype(&Foo::f)>::invoke<&Foo::f>(foo, 20);
  // std::cout << ret << std::endl;

  reflect::Factory::Register<Foo>("Foo")
      .addMemberVar("base", &Foo::base_)
      .addMemberVar("name", &Foo::name);

  if (auto object = reflect::Factory::getObject<Foo>("Foo")) {
    std::cout << "Foo: " << std::endl;
    for (auto& prop : object->getProperties())
      std::cout << "name: " << prop.getName() << std::endl;
  }

  // for (auto& meth : object.get_methods())
  //   std::cout << "name: " << meth.get_name();
}

int main() {
  test();

  return 0;
}
