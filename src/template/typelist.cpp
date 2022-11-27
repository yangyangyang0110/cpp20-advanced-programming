/* Proj: cpp20-advanced-programming
 * File: typelist.cpp
 * Created Date: 2022/11/16
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/16 22:54:20
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <tuple>
#include <type_traits>

template <typename... Ts>
struct TypeList {
  struct IsTypeList {};
  using type = TypeList;

  static constexpr size_t size = sizeof...(Ts);

  template <typename... T>
  using append = TypeList<Ts..., T...>;

  template <typename... T>
  using prepend = TypeList<T..., Ts...>;

  template <template <typename...> typename T>
  using to = T<Ts...>;
};

template <typename T>
concept TL = requires {
               typename T::IsTypeList;
               typename T::type;
             };

/**
 * @brief 实现基本高阶函数, map filter, fold
 */

template <TL types, template <typename> class F>
struct Map;

template <template <typename> class F, typename... Types>
struct Map<TypeList<Types...>, F> : TypeList<F<Types...>> {};

using Yes = char;
using No = bool;

#define NARGS(_0, _1, _2, _3, _4, _5, _6, _7, n, ...) n
#define PP_NARGS(...) NARGS(0, ##__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)

#define MACRO(...) \
  std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

template <typename... Fs>
void print_functions(Fs&&... ts) {
  ((std::cout << ts << std::endl), ...);
}

void test() {
  static_assert(std::is_same_v<
                TypeList<int, long, double>,
                TypeList<int>::append<long, double>>);
  static_assert(std::is_same_v<
                TypeList<int, long>::prepend<char, float>,
                TypeList<char, float>::append<int, long>>);
  static_assert(std::is_same_v<
                TypeList<int, long, double>::to<std::tuple>,
                std::tuple<int, long, double>>);

  // test map
  using Type = TypeList<char, int, double, long>;
  // static_assert(std::is_same_v<TypeLiost<>>);

  // MACRO(1, 2, 3)
  // MACRO(1)
  // MACRO()

  static_assert(PP_NARGS() == 0);
  static_assert(PP_NARGS() == MACRO());
  static_assert(PP_NARGS(1) == MACRO(3));
  static_assert(PP_NARGS(1, 2, 3) == MACRO(1, 2, 3));

  // print_functions(PP_NARGS(), PP_NARGS(1), PP_NARGS(3, 4, 5));
  // std::cout << PP_NARGS() << std::endl;
  // std::cout << PP_NARGS(1) << std::endl;
  // std::cout << PP_NARGS(3, 4, 5) << std::endl;
}

int main() {
  test();
  return 0;
}
