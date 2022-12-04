/* Proj: cpp20-advanced-programming
 * File: excepted.hpp
 * Created Date: 2022/11/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/20 08:13:52
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_EXCEPTED_HPP
#define CPP20_ADVANCED_PROGRAMMING_EXCEPTED_HPP

#include <optional>
#include <stdexcept>
#include <variant>

/**
 * @brief 如何约束T的能力集呢?
 * @tparam T
 * @tparam E
 */
template <class T, class E>
struct Excepted {
  template <typename... Args>
  constexpr static inline Excepted<T, E> success(Args&&... args) {
    Excepted result;
    result.has_value_ = true;
    new (&result.value_) T(std::forward<Args>(args)...);
    return result;
  }

  template <typename... Args>
  constexpr static inline Excepted<T, E> failure(Args&&... args) {
    Excepted result;
    result.has_value_ = false;
    new (&result.error_) E(std::forward<Args>(args)...);
    return result;
  }

public:
  constexpr const T* operator->() const noexcept {
    if (!has_value_)
      return nullptr;
    return &value_;
  }

  constexpr T* operator->() noexcept {
    if (!has_value_)
      return nullptr;
    return &value_;
  }

  constexpr T& value() noexcept(false) {
    if (!has_value_)
      throw std::logic_error("No value");
    return value_;
  }

  constexpr E& error() noexcept(false) {
    if (has_value_)
      throw std::logic_error("No error");
    return error_;
  }

  operator bool() const noexcept { return has_value_; }

  operator std::optional<T>() const noexcept {
    return has_value_ ? std::make_optional<T>(value_) : std::nullopt;
  }

public:
  Excepted()
      : has_value_(false){

        };

  Excepted(const Excepted& other) : has_value_(other.has_value_) {
    if (other.has_value_)
      new (&value_) T(other.value_);
    else
      new (&error_) E(other.error_);
  }

  Excepted(Excepted&& other) noexcept : has_value_(other.has_value_) {
    // std::cout << "Excepted Move Constructor" << std::endl;
    if (other.has_value_)
      new (&value_) T(std::move(other.value_));
    else
      new (&error_) E(std::move(other.error_));
  }

  // 使用副本去做swap, 保证数据一致性.
  Excepted& operator=(Excepted other) {
    // std::cout << "Excepted CSP" << std::endl;
    swap(other);
    return *this;
  }

  ~Excepted() noexcept {
    if (has_value_) {
      value_.~T();
    } else {
      error_.~E();
    }
  };

private:
  // copy and swap
  void swap(Excepted& other) {
    using std::swap;
    if (has_value_) {
      if (other.has_value_) {
        swap(value_, other.value_);
      } else {
        // copy error.
        auto error_clone = std::move(other.error_);
        other.error_.~E();

        new (&other.value_) T(std::move(value_));
        value_.~T();

        new (&error_) E(std::move(error_clone));

        swap(has_value_, other.has_value_);
      }
    } else {
      if (other.has_value_) {
        other.swap(*this); // 递归调用.
      } else {
        swap(other.error_, other.error_);
      }
    }
  }

private:
  bool has_value_;
  union {
    T value_;
    E error_;
  };
};

// 好蠢
template <typename T, typename E>
Excepted<T, E> make_excepted(T&& value) {
  return Excepted<T, E>::success(std::forward<T>(value));
}

template <typename T, typename E>
Excepted<T, E> make_excepted(E&& error) {
  return Excepted<T, E>::failure(std::forward<E>(error));
}

template <typename MyExcepted>
MyExcepted make_excepted(MyExcepted&& except) {
  return std::forward<MyExcepted>(except);
}

#if 0
// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T, typename E>
Excepted<T, E> make_excepted(const std::variant<T&&, E&&>& var) {
  std::visit(
      overloaded{
          [](T&& value) { return Excepted<T, E>::success(std::move(value)); },
          [](E&& error) { return Excepted<T, E>::success(std::move(error)); }},
      var);
}
#endif

template <
    typename F,
    typename Ret = std::result_of_t<F()>,
    typename Except = Excepted<Ret, std::exception_ptr>>
Except m_try(const F& f) noexcept {
  try {
    return make_excepted(f());
  } catch (...) {
    return make_excepted(std::current_exception());
  }
}

template <typename T, typename Variant, typename Except = Excepted<T, std::exception_ptr>>
Except m_get_if(const Variant& variant) noexcept {
  auto* p = std::get_if<T>(variant);
  return p ? make_excepted(p) : make_excepted("Variant not found the desired type.");
}

template <typename T, typename E>
T& m_get_or_throw(Excepted<T, E>& except) noexcept(false) {
  if (except) {
    except.value();
  } else {
    std::rethrow_exception(except.error());
  }
}

template <typename T, typename E, typename Except = Excepted<T, E>>
T* m_get_if(Except&& except) noexcept {
  return except ? except.value() : nullptr;
}

#endif // CPP20_ADVANCED_PROGRAMMING_EXCEPTED_HPP
