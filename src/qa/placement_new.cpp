#include <algorithm>
#include <iostream>
#include <string>
/* Proj: cpp20-advanced-programming
 * File: placement_new.cpp
 * Created Date: 2022/11/12
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/12 13:05:35
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

struct NonAssignable {
  NonAssignable() = default;
  NonAssignable& operator=(const NonAssignable&) = delete;
  // NonAssignable &operator=(NonAssignable &&) noexcept = delete;
};

struct Foo : NonAssignable {
  explicit Foo(std::string s)
      : value(std::move(s)){

        };

  Foo(Foo const& other) : value(other.value) {
    std::cout << "CopyConstructor" << std::endl;
  }

  Foo(Foo&& other) noexcept : value(std::move(other.value)) {
    std::cout << "MoveConstructor" << std::endl;
  }

  ~Foo() noexcept { std::cout << "Destructor" << std::endl; };

  friend std::ostream& operator<<(std::ostream& os, Foo const& ins) noexcept {
    return os << "value: " << ins.value;
  }

 private:
  std::string value;
};

struct Wrapper {
  static Wrapper make(std::string value) {
    Wrapper wrapper;
    new (&wrapper.foo_) Foo(std::move(value));
    return wrapper;
  }

  friend std::ostream& operator<<(
      std::ostream& os, Wrapper const& ins) noexcept {
    return os << "foo: " << ins.foo_;
  }

 public:
  Wrapper() = default;
  ~Wrapper() noexcept = default;

 private:
  Foo foo_{""};
};

void test() {
  auto wrap = Wrapper::make("Wrapper");
  Foo foo1("foo1"), foo2("foo2");
  std::cout << ">>> Starting..." << std::endl;
  new (&foo1) Foo(std::move(foo2)); // Only MoveConstructor.
  std::cout << "<<< End..." << std::endl;
  // foo1 = std::move(foo2);
}

int main() {
  test();
  return 0;
}
