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
struct Map {};




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
}

int main() {
  test();
  return 0;
}
