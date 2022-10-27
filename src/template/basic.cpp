/* Proj: cpp20-advanced-programming
 * File: basic.cpp
 * Created Date: 2022/10/16
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/16 23:40:30
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include <type_traits>

namespace detail {
template <typename T> constexpr bool always_false = false;
template <typename T> T &&declval() noexcept
{
    static_assert(always_false<T>, "declval not allowed in an evaluated context");
}

} // namespace detail

void test() { static_assert(std::is_same_v<decltype(detail::declval<int>()), int &&>); }

int main()
{
    test();
    return 0;
}