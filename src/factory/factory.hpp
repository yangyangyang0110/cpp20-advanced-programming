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

#include <any>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include "helper/type_trait_extends.hpp"

#include "helper/excepted.hpp"

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

template <class Class>
struct ClassProxy {
  template <typename... Args>
  std::optional<Class> operator()(Args&&... args) noexcept {
    if constexpr (std::is_constructible_v<Class, Args...>) {
      if constexpr (std::is_nothrow_constructible_v<Class, Args...>) {
        return Class(std::forward<Args>(args)...);
      } else {
        return std::nullopt;
      }
    } else {
      static_assert(is_always_false_v<Class>, "Class must has constructor.");
    }
  }
};

class Factory {
public:
  using Creator = std::any;
  using CreatorRegistry = std::map<const std::string, Creator>;

  template <
      class Class,
      typename... Args,
      typename R = Excepted<Class, std::string>,
      std::enable_if_t<std::is_constructible_v<Class, Args...>, int> = 0>
  static R getObject(const std::string& name, Args&&... args) {
    auto creatorRegistry = getCreatorRegistry();
    if (auto it = creatorRegistry.find(name); it != creatorRegistry.end()) {
      try {
        // std::cout << "start classProxy: " << std::endl;
        auto classProxy = std::any_cast<ClassProxy<Class>>(it->second);
        // std::cout << "end classProxy: " << std::endl;
        if (auto opt = classProxy(std::forward<Args>(args)...)) {
          return R::success(std::move(opt.value()));
        } else {
          return R::failure("Failed to create class: " + name);
        }
      } catch (const std::bad_any_cast& e) {
        std::cerr << e.what() << std::endl;
        return R::failure(e.what());
      }
    }
    return R::failure("Can not find Class `" + name + "`");
  }

  template <class Class>
  static bool registry(const std::string& name, ClassProxy<Class>&& classProxy) noexcept {
    printf(">>> addCreator, name: %s\n", name.c_str());
    getCreatorRegistry()[name] = std::forward<ClassProxy<Class>>(classProxy);
    return true;
  }

private:
  static CreatorRegistry& getCreatorRegistry() noexcept {
    static CreatorRegistry creatorRegistry;
    return creatorRegistry;
  }
};

#endif

#define REGISTER_CLASS(type) bool b##type = Factory::registry(#type, ClassProxy<type>{})

// #define REGISTER_CLASS(type) \
//   bool b##type =             \
//       Factory::registry(#type, (Factory::Creator)(&std::make_shared<type>))

#endif // CPP20_ADVANCED_PROGRAMMING_FACTORY_HPP
