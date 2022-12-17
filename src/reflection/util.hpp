/* Proj: cpp20-advanced-programming
 * File: util.hpp
 * Created Date: 2022/12/17
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/17 13:03:58
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_UTIL_HPP
#define CPP20_ADVANCED_PROGRAMMING_UTIL_HPP

#include <any>
#include <cassert>
#include <functional>
#include <map>
#include <optional>

namespace reflect {

template <class Class, typename T>
struct PropertyImpl {
  PropertyImpl(T Class::*memberPointer, char const* memberName)
      : m_MemberPointer(memberPointer), m_MemberName(memberName) {}

private:
  T Class::*m_MemberPointer;
  char const* m_MemberName;
};

class MemberVar {
public:
  MemberVar() noexcept = default;

  template <typename T, class Class>
  explicit MemberVar(char const* memberName, T Class::*memberPointer) noexcept
      : name_(memberName) {
    getter_ = [memberPointer](std::any obj) -> std::any {
      return std::any_cast<const Class*>(obj)->*memberPointer;
    };

    setter_ = [memberPointer](std::any obj, std::any val) -> bool {
      auto* objPtr = std::any_cast<Class*>(obj);
      // TODO: 如何传递值类别?
      objPtr->*memberPointer = std::any_cast<T>(val);
      return true;
    };
  }

public:
  template <typename T, class Class>
  T getValue(const Class& obj) const noexcept {
    return std::any_cast<T>(getter_(obj));
  }

  template <typename T, class Class>
  bool setValue(Class& obj, T&& val) noexcept {
    return setter_(&obj, std::forward<T>(val));
  }

  [[nodiscard]] char const* getName() const noexcept { return name_; }

private:
  std::function<bool(std::any, std::any)> setter_;
  std::function<std::any(std::any)> getter_;
  char const* name_{};
};

class MemberFunc {};

template <class Class>
class Object {
public:
  using Self = Object<Class>;
  using SelfRef = Self&;

public:
  template <typename T, typename R = PropertyImpl<Class, T>>
  SelfRef addMemberVar(char const* memberName, T Class::*memberPointer) noexcept {
    MemberVar property(memberName, memberPointer);
    memberVars_.push_back(std::move(property));
    return *this;
  }

public:
  template <typename T>
  bool setValue(char const* varName, T&& val) noexcept {
    for (auto&& memberVar : memberVars_) {
      if (memberVar.getName() == varName) {
        // return memberVar.setValue()
      }
    }
    return false;
  }

public:
  [[nodiscard]] std::vector<MemberVar>& getProperties() noexcept { return memberVars_; }

  [[nodiscard]] std::vector<MemberFunc>& getMethods() noexcept { return memberFuncs_; }

  [[nodiscard]] const std::vector<MemberVar>& getProperties() const noexcept {
    return memberVars_;
  }
  [[nodiscard]] const std::vector<MemberFunc>& getMethods() const noexcept {
    return memberFuncs_;
  }

private:
  std::vector<MemberVar> memberVars_;
  std::vector<MemberFunc> memberFuncs_;
};

class Factory {
public:
  template <class Class, typename R = Object<Class>&>
  static R Register(char const* className) noexcept {
    if (classMap_.find(className) != classMap_.end()) {
      assert(false);
    }
    classMap_.emplace(className, Object<Class>());
    return std::any_cast<R>(classMap_[className]);
  }

  template <class Class, typename R = Object<Class>>
  static std::optional<R> getObject(char const* className) noexcept {
    auto it = classMap_.find(className);
    return (it != classMap_.end()) ? std::make_optional(std::any_cast<R>(it->second))
                                   : std::nullopt;
  }

private:
  static inline std::map<char const*, std::any> classMap_;
};

} // namespace reflect

template <class Class>
struct MemberFunction;

template <class Class, class Ret, class... Args>
struct MemberFunction<Ret (Class::*)(Args...)> {
  template <Ret (Class::*p)(Args...)>
  static Ret invoke(Class& obj, Args&&... args) {
    return (obj.*p)(std::forward<Args>(args)...);
  }
};

template <class Class, class Ret, class... Args>
struct MemberFunction<Ret (Class::*)(Args...) const> {
  template <Ret (Class::*p)(Args...) const>
  static Ret invoke(const Class& obj, Args&&... args) {
    return (obj.*p)(std::forward<Args>(args)...);
  }
};

template <class Class, typename RetType, typename... Args>
struct MemberFunction<RetType (Class::*)(Args...) const noexcept> {
  template <RetType (Class::*p)(Args...) const noexcept>
  static RetType invoke(const Class& self, Args&&... args) noexcept {
    return (self.*p)(std::forward<Args>(args)...);
  }
};

#endif // CPP20_ADVANCED_PROGRAMMING_UTIL_HPP
