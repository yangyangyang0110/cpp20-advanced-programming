/* Proj: cpp20-advanced-programming
 * File: product-type.cpp
 * Created Date: 2022/11/21
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/21 22:57:07
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct Visit {
  void operator()(const int& x) { std::cout << x << std::endl; }

  void operator()(const double& x) { std::cout << x << std::endl; }
};

void test() {
  using Type = std::variant<int, double>;
  std::vector<Type> data{1, 2.2};
  for (const auto& it : data) {
    std::visit(Visit{}, it);
  }
}

int main() {
  test();
  return 0;
}
