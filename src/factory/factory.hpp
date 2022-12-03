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

#if 0
class Factory {
public:
  // using Creator = std::function<std::shared_ptr<Inference>()>;
  using Creator = void*;
  using CreatorRegistry = std::map<const std::string, Creator>;

  template <typename T, typename... Args>
  static std::shared_ptr<T> create(const std::string_view name, Args&&... args) noexcept {
    using CreatorFunction = std::shared_ptr<T> (*)(Args && ...);
    auto creatorRegistry = getCreatorRegistry();
    auto it = creatorRegistry.find(std::string(name));
    return it != creatorRegistry.end()
        ? reinterpret_cast<CreatorFunction>(it->second)(std::forward<Args>(args)...)
        : nullptr;
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
#else

class Factory {
public:
  // using Creator = std::function<std::shared_ptr<Inference>()>;
  using Creator = void*;
  using CreatorRegistry = std::map<const std::string, Creator>;
  using CreatorTypes = std::map<std::string, std::tuple<int>>;

  template <typename T, typename... Args>
  static std::shared_ptr<T> getObject(
      const std::string_view name, Args&&... args) noexcept {
    using CreatorFunction = std::shared_ptr<T> (*)(Args && ...);
    auto creatorRegistry = getCreatorRegistry();
    auto it = creatorRegistry.find(std::string(name));
    return it != creatorRegistry.end()
        ? reinterpret_cast<CreatorFunction>(it->second)(std::forward<Args>(args)...)
        : nullptr;
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

#endif
#define REGISTER_CLASS(type, ...) \
  bool b##type =                  \
      Factory::registry(#type, (Factory::Creator)(&std::make_shared<type, __VA_ARGS__>))

// #define REGISTER_CLASS(type) \
//   bool b##type =             \
//       Factory::registry(#type, (Factory::Creator)(&std::make_shared<type>))

#endif // CPP20_ADVANCED_PROGRAMMING_FACTORY_HPP
