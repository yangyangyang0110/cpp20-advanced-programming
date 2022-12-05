/* Proj: cpp20-advanced-programming
 * File: factory.hpp
 * Created Date: 2022/11/17
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/17 23:14:53
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_FACTORY_HPP
#define CPP20_ADVANCED_PROGRAMMING_FACTORY_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include "helper/type_trait_extends.hpp"

#include "helper/excepted.hpp"

template <class Class, typename R = Excepted<Class, std::string>>
struct ClassProxy {
  template <typename... Args>
  static R invoke(Args&&... args) noexcept {
    if constexpr (std::is_constructible_v<Class, Args...>) {
      if constexpr (std::is_nothrow_constructible_v<Class, Args...>) {
        return R::success(Class(std::forward<Args>(args)...));
      } else {
        try {
          return R::success(Class(std::forward<Args>(args)...));
        } catch (const std::exception& e) {
          return R::failure(e.what());
        }
      }
    } else {
      static_assert(is_always_false_v<Class>, "Class must has constructor.");
    }
  }
};

class Factory {
public:
  using Creator = void*;
  using CreatorRegistry = std::map<const std::string, Creator>;

  // 类型不安全.
  template <
      class Base,
      typename... Args,
      typename R = Excepted<std::shared_ptr<Base>, std::string>>
  static R getObject(const std::string& name, Args&&... args) {
    using CreatorType = R (*)(Args...);
    auto creatorRegistry = getCreatorRegistry();
    if (auto it = creatorRegistry.find(name); it != creatorRegistry.end()) {
      return reinterpret_cast<CreatorType>(it->second)(std::forward<Args>(args)...);
    }
    return R::failure("Can not find Class `" + name + "`");
  }

  static bool registry(const std::string& name, Creator creator) noexcept {
    printf(">>> addCreator, name: %s\n", name.c_str());
    getCreatorRegistry()[name] = creator;
    return true;
  }

private:
  static CreatorRegistry& getCreatorRegistry() noexcept {
    static CreatorRegistry creatorRegistry;
    return creatorRegistry;
  }
};

// template <
//     class Class,
//     typename... Args,
//     typename R = Excepted<std::shared_ptr<Class>, std::string>,
//     std::enable_if<std::is_nothrow_constructible_v<Class, Args...>, int> = 0>
// R makeShared(Args&&... args) noexcept {
//   return R::success(std::make_shared<Class>(std::forward<Args>(args)...));
// }

template <
    class Class,
    typename... Args,
    typename R = Excepted<std::shared_ptr<Class>, std::string>>
R makeShared(Args&&... args) noexcept {
  try {
    return R::success(std::make_shared<Class>(std::forward<Args>(args)...));
  } catch (const std::exception& e) {
    return R::failure(e.what());
  }
}

#define REGISTER_CLASS(type, ...) \
  bool _##type = Factory::registry(#type, (void*)(&makeShared<type, __VA_ARGS__>))

#endif // CPP20_ADVANCED_PROGRAMMING_FACTORY_HPP
