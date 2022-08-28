/**
 * @file 02_coroutine_generator.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <coroutine>
#include <iostream>
#include <type_traits>

/**
 * @brief 
 * @FAQ
 *  1. 如何改进值copy
 *  2. Generator 提供 iterator 迭代方式.
 *  3. 
 * 
 * @tparam T 
 */
template<typename T>
struct Generator {
  struct promise_type {
    // 存储协程产出的值, 并提供给get接口供外部调用.
    const T &result() const noexcept { return value_; }

    template<std::convertible_to<T> From> // C++20 concept
    std::suspend_always yield_value(From &&from) noexcept {
      value_ = std::forward<From>(from); // caching the result in promise
      return {};
    }

    Generator get_return_object() noexcept {
      return Generator{
          std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() const noexcept { return {}; }

    void unhandled_exception() noexcept {}

    std::suspend_always final_suspend() noexcept { return {}; }

    ~promise_type() { std::cout << "~promise_type\n"; }

  private:
    T value_; // 不合理的设计.
  };

  explicit Generator(std::coroutine_handle<promise_type> handle) noexcept
      : handle_(handle) {}


  ~Generator() {
    std::cout << "~Generator\n";
    if (handle_) {
      std::cout << "handle_.destroy()\n";
      handle_.destroy();
    }
  }

  // ps: 生命周期(lifetime)问题.
  const T &result() const noexcept { return handle_.promise().result(); }
  void next() { handle_.resume(); }


private:
  // std::coroutine_handle 的本质是指向协程帧的指针，通常按值传递。
  std::coroutine_handle<promise_type> handle_;

  // TODO: begin() end() operator*
};

Generator<int> fibonacci(int a0, int a1) {
  co_yield a0;
  co_yield a1;

  for (int a2; true;) {
    a2 = a0 + a1;
    a0 = a1;
    a1 = a2;
    co_yield a2;
  }
}


int main() {
  auto fibo = fibonacci(1, 1);
  for (int i = 0; i < 10; ++i) {
    fibo.next();
    std::cout << fibo.result() << std::endl;
  }
  return 0;
}
