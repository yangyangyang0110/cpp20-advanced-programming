/**
 * @file 01_generator.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-08-27
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include <type_traits>

using i32 = int;

template <
    typename T = i32,
    std::enable_if_t<
        std::is_integral_v<T> || std::is_floating_point_v<T>,
        void*> = nullptr>
struct FibonacciGenerator {
  explicit FibonacciGenerator(T&& a0 = 1, T&& a1 = 1) : a0_(a0), a1_(a1) {}
  T operator()() {
    T a2 = a0_ + a1_;
    a0_ = a1_;
    a1_ = a2;
    return a2;
  }

 private:
  T a0_, a1_;
};

void test() {
  FibonacciGenerator fib;
  for (int i = 0; i < 10; ++i) {
    std::cout << i << " -> " << fib() << std::endl;
  }
}

int main() {
  test();
  return 0;
}
