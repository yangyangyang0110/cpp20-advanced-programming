/* Proj: cpp20-advanced-programming
 * File: base.hpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 13:55:44
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_BASE_HPP
#define CPP20_ADVANCED_PROGRAMMING_BASE_HPP

class Base {
public:
  virtual ~Base() noexcept = default;
  virtual void Update(void*) noexcept = 0;
};

#endif // CPP20_ADVANCED_PROGRAMMING_BASE_HPP
