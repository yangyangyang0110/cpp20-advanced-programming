/* Proj: cpp20-advanced-programming
 * File: fibonacci.cpp
 * Created Date: 2022/10/16
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/16 23:34:07
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <cstdlib>
#include <iostream>
#include <numeric>

template <size_t N> constexpr size_t fibonacci = fibonacci<N - 1> + fibonacci<N - 2>;
template <> constexpr size_t fibonacci<0> = 0;
template <> constexpr size_t fibonacci<1> = 1;

template <size_t N> constexpr double golden_ratio = fibonacci<N + 1> * 1.0 / fibonacci<N>;

int main()
{
    static_assert(fibonacci<10> == 55);
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    std::cout << golden_ratio<10> << std::endl;
    std::cout << golden_ratio<20> << std::endl;
    return 0;
}