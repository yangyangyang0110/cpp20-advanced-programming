/* Proj: cpp20-advanced-programming
 * File: test-index_sequence.cpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 11:29:41
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <cstdlib>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>

enum class Classes { Car, Person, Phone, Computer };

template <size_t... I>
void printEnum() {
  // std::cout << ((static_cast<int32_t>(Classes(I))), ...) << std::endl;
  ((std::cout << static_cast<int32_t>(Classes(I)) << " "), ...) << std::endl;
}

template <typename T, T... Idx, typename F>
void call(std::integer_sequence<T, Idx...>, F&& f) {
  (f(std::integral_constant<T, Idx>{}), ...);
}

template <typename Tuple>
void print(const Tuple& t) {
  constexpr auto N = std::tuple_size_v<Tuple>;
  // create index sequence and loop.
  call(std::make_index_sequence<N>{}, [&](auto i) {
    std::cout << static_cast<int32_t>(std::get<i>(t))
              << ((decltype(i)::value != N - 1) ? ", " : ";");
  });
}

void test() {
  printEnum<0, 2, 3>();
  constexpr auto t = std::make_tuple(Classes::Car, Classes::Person);
  print(t);
}

int main() {
  test();
  return 0;
}
