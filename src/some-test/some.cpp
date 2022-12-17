/* Proj: cpp20-advanced-programming
 * File: some.cpp
 * Created Date: 2022/12/6
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/6 21:23:35
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include "some.hpp"
#include <iostream>

template <typename T>
void fImpl(T&& value) {
  std::cout << "Impl: " << value << std::endl;
}

#define GEN(type)                      \
  template <>                          \
  bool f<const type&>(const type& v) { \
    fImpl(v);                          \
  }                                    \
  template <>                          \
  bool f<type>(type && v) {            \
    fImpl(std::move(v));               \
  }

GEN(int);
GEN(double);
