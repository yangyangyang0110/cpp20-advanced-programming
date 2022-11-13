/**
 * @file 00_demo.cpp
 * @brief
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <coroutine>
#include <iostream>

template <typename T, size_t N>
size_t getArraySize(T (&)[N]) {
  return N;
}

struct promise;
struct coroutine : std::coroutine_handle<promise> {
  using promise_type = struct promise;
};

struct promise {
  coroutine get_return_object() { return {coroutine::from_promise(*this)}; }
  std::suspend_always initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void return_void() {}
  void unhandled_exception() {}
};

struct S {
  int i;
  coroutine f() {
    std::cout << i;
    co_return;
  }
};

void bad1() {
  coroutine h = S{1}.f();
  // S{0} destroyed
  h.resume(); // resumed coroutine executes stl::cout << i, uses S::i after free
  h.destroy();
}

coroutine bad2() {
  S s{2};
  return s.f(); // returned coroutine can't be resumed without committing use
                // after free
}

void bad3() {
  coroutine h = [i = 3]() -> coroutine { // a lambda that's also a coroutine
    std::cout << i;
    co_return;
  }(); // immediately invoked
  // lambda destroyed
  h.resume(); // uses (anonymous lambda type)::i after free
  h.destroy();
}

void good() {
  coroutine h = [](int i) -> coroutine { // make i a coroutine parameter
    std::cout << i;
    co_return;
  }(4);
  // lambda destroyed
  h.resume(); // no problem, i has been copied to the coroutine frame as a
              // by-value parameter
  h.destroy();
}

void test() {
  std::cout << "Before bad1" << std::endl;
  bad1();
  std::cout << std::endl;

  std::cout << "Before bad2" << std::endl;
  auto c = bad2();
  c.resume();
  c.destroy();
  std::cout << std::endl;

  std::cout << "Before bad3" << std::endl;
  bad3();
  std::cout << std::endl;

  std::cout << "Before good" << std::endl;
  good();
  std::cout << std::endl;
}

int main() {
  test();
  return 0;
}
