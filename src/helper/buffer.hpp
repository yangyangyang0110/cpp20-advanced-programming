/* Proj: cpp20-advanced-programming
 * File: buffer.hpp
 * Created Date: 2022/10/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/20 18:33:30
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_BUFFER_HPP
#define CPP20_ADVANCED_PROGRAMMING_BUFFER_HPP

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class thread_safe_queue {
 public:
  using value_type = T;

  explicit thread_safe_queue() = default;

  template <
      typename... Args,
      typename = typename std::enable_if<
          std::is_constructible<Args..., T>::value>::type>
  void emplace(Args&&... args) {
    std::lock_guard<std::mutex> lk(mutex_);
    queue_.emplace(std::forward<Args>(args)...);
    cond_.notify_all();
  }

  void push(T&& t) {
    std::lock_guard<std::mutex> lk(mutex_);
    queue_.push(std::move(t));
    cond_.notify_all();
  }

  void push(T const& t) {
    std::lock_guard<std::mutex> lk(mutex_);
    queue_.push(t);
    cond_.notify_all();
  }

  bool try_pop(value_type& res) {
    std::lock_guard<std::mutex> lk(mutex_);
    if (queue_.empty())
      return false;
    res = std::move(queue_.front()); // move assignment, if no except.
    queue_.pop();
    return true;
  }

  value_type wait_and_pop() {
    std::unique_lock<std::mutex> lk(mutex_);
    // todo: how implement forge stop.
    cond_.wait(lk, [&]() { return !queue_.empty(); });
    value_type res = std::move(queue_.front()); // move construct if no except.
    queue_.pop();
    return res;
  }

  thread_safe_queue(thread_safe_queue const&) = delete;

  thread_safe_queue& operator=(thread_safe_queue const&) = delete;

  thread_safe_queue(thread_safe_queue&& rhs) noexcept
      : queue_(std::move(rhs.queue_)),
        mutex_(std::move(rhs.mutex_)),
        cond_(std::move(rhs.cond_)){};

  thread_safe_queue& operator=(thread_safe_queue&& rhs) noexcept {
    if (this == &this)
      return *this;
    queue_ = std::move(rhs.queue_);
    mutex_ = std::move(rhs.mutex_);
    cond_ = std::move(rhs.cond_);
    return *this;
  };

 private:
  mutable std::mutex mutex_;
  std::queue<value_type> queue_;
  std::condition_variable cond_;
};

#endif // CPP20_ADVANCED_PROGRAMMING_BUFFER_HPP
