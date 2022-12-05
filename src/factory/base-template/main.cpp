/* Proj: cpp20-advanced-programming
 * File: main.cpp
 * Created Date: 2022/11/17
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/17 23:14:15
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include "factory.hpp"
#include "strategy/mod.hpp"

#define FACTORY_CREATE(type, ...) Factory::getObject<type>(#type, __VA_ARGS__)

#define COUNT_ARGS(...) \
  std::cout << std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))> << std::endl

int main() {
  if (auto exc = Factory::getObject<MaybeThrow>("MaybeThrow", false); !exc) {
    std::cerr << exc.error() << std::endl;
  }

  if (auto exc = Factory::getObject<MaybeThrow>("MaybeThrow", true); !exc) {
    std::cerr << exc.error() << std::endl;
  }

  if (auto exc = Factory::getObject<CallSmoke>("CallPhone", 10)) {
    exc->update(CallSmokeFrameData{});
  } else {
    std::cerr << exc.error() << std::endl;
  }

  if (auto exc = Factory::getObject<CallSmoke>("Call", 10)) {
    exc->update(CallSmokeFrameData{});
  } else {
    std::cerr << exc.error() << std::endl;
  }

  if (auto exc = FACTORY_CREATE(CallPhone, CallPhoneParam{10, "desc"})) {
    exc->update(CallPhoneFrameData{});
  } else {
    std::cerr << exc.error() << std::endl;
  }

  return 0;
}
