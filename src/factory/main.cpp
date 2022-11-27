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

#define COUNT_ARGS(...)                                    \
  std::cout << std::tuple_size_v<decltype(std::make_tuple( \
                   __VA_ARGS__))> << std::endl

struct Foo {
  Foo(int a, int b) noexcept : a(a), b(b) {}

  friend std::ostream& operator<<(std::ostream& os, const Foo& foo) noexcept {
    return os << "a: " << foo.a << " b: " << foo.b;
  }

private:
  int a{}, b{};
};

int main() {
  Foo foo(10, 20);
  std::cout << foo << std::endl;

  // auto foo = call<Foo>(10, 20);
  // std::cout << foo << std::endl;

  // std::map<std::string, Curried> maps{
  //     {"foo", }};

  // auto r = maps.at("foo").operator()(3, 4);
  // std::cout << r << std::endl;

  // auto o = Factory::create<CallSmoke, int>("CallSmoke", 10);
  // o->update();

  // auto o2 = CREATE_V(CallSmoke, 10);
  // o2->update();

  // auto o1 = CREATE_V(CallPhone, CallPhoneParam{});
  // auto o1 = CREATE_V(CallPhone, 100);
  // o1->update();
  // object->Preprocess(true);
  // COUNT_ARGS(1, "12312", "asdasda");

  return 0;
}
