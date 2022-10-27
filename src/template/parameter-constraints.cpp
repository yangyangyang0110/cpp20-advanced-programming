/* Proj: cpp20-advanced-programming
 * File: parameter-constraints.cpp
 * Created Date: 2022/10/16
 * Author: YangYangYang (yangzilong@objecteye.com)
 * Description:
 * -----
 * Last Modified: 2022/10/16 22:46:55
 * Modified By: YangYangYang (yangzilong@objecteye.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <concepts>
#include <iostream>

/**
 * @brief todo: 如何使用模版对形参的值进行约束
 */

template <int x, int min = 0, int max = 10>
inline constexpr bool is_valid_value = (x >= min && x <= max);

template <int x, int min = 0, int max = 10>
concept is_valid_value_cpp20 = requires { x >= min &&x <= max; };

template <int x, typename std::enable_if<is_valid_value<x>, void *>::type = nullptr>
void f() noexcept
{
    std::cout << "x = " << x << std::endl;
}

void test() { f<1>(); }

int main()
{
    test();
    return 0;
}
