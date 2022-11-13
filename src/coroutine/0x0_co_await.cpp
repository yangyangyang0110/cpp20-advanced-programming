/*
 * File: 0x0_co_await.cpp
 * Project: coroutine
 * File Created: 2022-08-28 13:03:07
 * Author: yangyangyang (yangyangyang0110@outlook.com)
 * -----
 * Last Modified: 2022-08-28 13:20:44
 * Modified By: yangyangyang (yangyangyang0110@outlook.com>)
 * -----
 * Copyright 2022 - 2022  ,
 */
#include <coroutine>
#include <future>
#include <iostream>

struct Foo {
  void operator co_await() {}
};

template <typename T>
auto operator co_await(std::future<T> fut) {}

void test() {}

int main() {
  test();
  return 0;
}
