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

int main() {
  // auto o = Factory::create<CallSmoke, int>("CallSmoke", 10);
  // o->update();

  auto o2 = CREATE_V(CallSmoke, 10);
  o2->update();

  auto o1 = CREATE_V(CallPhone, CallPhoneParam{});
  // auto o1 = CREATE_V(CallPhone, 100);
  o1->update();
  // object->Preprocess(true);
  // COUNT_ARGS(1, "12312", "asdasda");

  return 0;
}
