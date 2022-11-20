/* Proj: cpp20-advanced-programming
 * File: json-.cpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 07:58:33
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <nlohmann/json.hpp>

#include "helper/excepted.hpp"

using nlohmann::json;
using namespace nlohmann::literals;

struct User {
  using type = User;
  using error_type = std::string;
  using excepted_type = Excepted<type, error_type>;

  std::string uname;
  std::string passwd;

  friend std::ostream& operator<<(std::ostream& os, const User& rhs) noexcept {
    return os << "[User]: " << rhs.uname << ": " << rhs.passwd << std::endl;
  }

  static excepted_type from_json(const json& data) noexcept {
    try {
      return make_excepted<type, error_type>(
          User{.uname = data["username"], .passwd = data["password"]});
    } catch (const std::exception& e) {
      // std::cerr << e.what() << std::endl;
      return make_excepted<type, error_type>(e.what());
    }
  }

  static Excepted<json, error_type> to_json(const User& user) noexcept {
    json data;
    data["username"] = user.uname;
    data["password"] = user.passwd;
    return make_excepted<json, error_type>(std::move(data));
  };
};

void test() noexcept(false) {
  json data = R"({
    "username": "I'm username",
    "password": "I'm password"
  })"_json;

  // 解析data为User对象.
  auto user = User::from_json(data);

  std::cout << user.value() << std::endl;

  std::cout << User::to_json(user.value()).value() << std::endl;
}

int main() {
  test();
  return 0;
}
