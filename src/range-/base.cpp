/* Proj: cpp20-advanced-programming
 * File: base.cpp
 * Created Date: 2022/10/25
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/25 00:15:19
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <ranges>

namespace ranges = std::ranges;
namespace views  = std::ranges::views;

void test()
{
    auto res = std::views::iota(1, 10) |
               std::views::transform([](const auto &n) { return n * n; }) |
               std::views::filter([](const auto &n) { return n % 2 == 1; });

    for (const auto j: res) {
        std::cout << j << std::endl;
    }


    // std::cout << std::sum(res) << std::endl;
}

int main()
{
    test();
    return 0;
}
