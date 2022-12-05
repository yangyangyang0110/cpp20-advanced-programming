/* Proj: cpp20-advanced-programming
 * File: call_phone.hpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 17:23:45
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_CALL_PHONE_HPP
#define CPP20_ADVANCED_PROGRAMMING_CALL_PHONE_HPP

#include <iostream>
#include <string>
#include "./base.hpp"

struct CallPhoneParam {
  std::string desc;
  int minimum_area{};

  friend std::ostream& operator<<(std::ostream& os, const CallPhoneParam& rhs) noexcept {
    return os << "minimum area: " << rhs.minimum_area << " desc: " << rhs.desc;
  }
};

struct CallPhoneFrameData {

};

class CallPhone : public Base {
public:
  CallPhone(CallPhoneParam&& param) noexcept : param_(std::move(param)) {}

  void Update(void *frameData) noexcept override {
    auto *param = reinterpret_cast<CallPhoneFrameData*>(frameData);
    std::cout << "CallPhone: " << param_ << std::endl;
  }

private:
  CallPhoneParam param_;
};

#endif // CPP20_ADVANCED_PROGRAMMING_CALL_PHONE_HPP
