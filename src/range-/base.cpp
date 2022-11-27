/* Proj: cpp20-advanced-programming
 * File: base.cpp
 * Created Date: 2022/10/25
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/25 00:15:19
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include "range/v3/algorithm.hpp"
#include "range/v3/numeric.hpp"
#include "range/v3/view.hpp"

auto is_six = [](int i) { return i == 6; };
using std::cout;

void test() {
  int sum = ranges::accumulate(
      ranges::views::ints(1, ranges::unreachable) |
          ranges::views::transform([](int i) { return i * i; }) | ranges::views::take(10),
      0);
  cout << sum << std::endl;

  std::vector<int> v{6, 2, 3, 4, 5, 6};
  cout << std::boolalpha;
  cout << "vector: " << ranges::views::all(v) << '\n';

  cout << "vector any_of is_six: " << ranges::any_of(v, is_six) << '\n';
  cout << "vector all_of is_six: " << ranges::all_of(v, is_six) << '\n';
  cout << "vector none_of is_six: " << ranges::none_of(v, is_six) << '\n';

  std::vector<int> const vi{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  using namespace ranges;
  auto rng = vi | views::filter([](int i) { return i % 2 == 0; }) |
      views::transform([](int i) { return std::to_string(i); });
  // prints: [2,4,6,8,10]
  cout << rng << '\n';
}

int main() {
  test();
  return 0;
}
