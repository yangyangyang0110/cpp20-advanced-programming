/* Proj: cpp-functional-programming
 * File: mbind.cpp
 * Created Date: 2022/10/28
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/28 00:06:04
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <optional>
#include <string>

// mbind(M<T1>, F: T1 -> M<T2>) -> M<T2>
template <typename T, typename F>
auto mbind(const std::optional<T>& opt, F f) -> decltype(f(opt.value())) {
  if (opt)
    return f(opt.value());
  else
    return {};
}

std::optional<std::string> add_header(std::optional<std::string> const& opt) {
  if (!opt)
    return {};
  return {"<h1>" + opt.value() + "</h1>"};
}

std::optional<std::string> to_user_html(std::optional<std::string> const& opt) {
  if (!opt)
    return {};
  return {opt.value() + "===> html"};
}

void test() {
  std::optional<std::string> str = "Hello";
  auto opt = mbind(mbind(str, add_header), to_user_html);
  if (opt) {
    std::cout << opt.value() << std::endl;
  }
}

int main() {
  test();
  return 0;
}