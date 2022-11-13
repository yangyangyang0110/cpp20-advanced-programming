#include <concepts>
#include <iostream>

template <typename T>
constexpr bool P = requires(typename T::value_type v) { ++v; };
template <typename T>
concept C = requires(typename T::value_type v) { ++v; };

template <typename T>
struct Foo {
  using value_type = T;
};

void test() {
  static_assert(P<Foo<int>>);
  static_assert(!P<Foo<std::string>>);
  static_assert(C<Foo<int>>);
  static_assert(!C<Foo<std::string>>);
}

int main() {
  test();
  return 0;
}
