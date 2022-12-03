/* Proj: cpp-functional-programming
 * File: for-future-folly.cpp
 * Created Date: 2022/11/13
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/13 22:14:24
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <folly/futures/Future.h>
#include <folly/Random.h>
#include <folly/Range.h>

using namespace folly;

void test() {
  // Future<int> f = Promise<int>().getFuture();
  // f.then([](int v) { return 10 + v; });
}

int main() {
  test();
  return 0;
}
