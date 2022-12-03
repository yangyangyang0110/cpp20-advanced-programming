/* Proj: cpp20-advanced-programming
 * File: thread_pool.hpp
 * Created Date: 2022/10/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/20 18:32:59
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#ifndef CPP20_ADVANCED_PROGRAMMING_THREAD_POOL_HPP
#define CPP20_ADVANCED_PROGRAMMING_THREAD_POOL_HPP

#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <vector>

#include "./buffer.hpp"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std::chrono_literals;

// Defined Move only semantic.
struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

template <typename F, typename... Args>
struct FArgs {
  explicit FArgs(F&& f, Args&&... args) noexcept
      : f_(std::forward<F>(f)), args_(std::make_tuple(std::forward<Args>(args)...)) {}

  std::invoke_result_t<F, Args...> invoke() {
    return std::apply(std::forward<F>(f_), std::move(args_));
  }

private:
  F&& f_;
  std::tuple<Args...> args_;
};

/**
 *
 */
class ThreadPool : public NonCopyable {
  using task_type = std::function<void()>;
  // using task_type = std::packaged_task<void()>;

  std::vector<std::thread> threads_;
  std::queue<task_type> task_queue_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cv_;
  std::atomic<bool> should_terminate_;
  std::atomic<int32_t> action_thread_;
  static inline std::shared_ptr<ThreadPool> instance_ = nullptr;
  static inline std::once_flag flag_;

public:
  static std::weak_ptr<ThreadPool> getWeakInstance() noexcept {
    std::call_once(flag_, initializeInstance);
    return instance_;
  }

  static ThreadPool* getUnsafeInstance() noexcept {
    std::call_once(flag_, initializeInstance);
    return (instance_ != nullptr) ? instance_.get() : nullptr;
  }

  static void release() noexcept { instance_.reset(); }

  template <typename F, typename... Args, typename R = std::invoke_result_t<F, Args...>>
  std::future<R> execute(F&& f, Args&&... args) {
    std::packaged_task<R()> task(
        [wrap = FArgs(std::forward<F>(f), std::forward<Args>(args)...)]() mutable -> R {
          return wrap.invoke();
        });
    auto fut = task.get_future();

    std::lock_guard lock(mutex_);
    // package_task 内部有shared_ptr, 暂时用 functional.
    if constexpr (std::is_same_v<task_type, std::packaged_task<void()>>) {
      task_queue_.emplace([task = std::move(task)]() mutable { task(); });
    } else {
      task_queue_.emplace([taskPtr = std::make_shared<decltype(task)>(
                               std::move(task))]() mutable { (*taskPtr)(); });
    }
    cv_.notify_all();
    return fut;
  }

public:
  ~ThreadPool() {
    should_terminate_.store(true, std::memory_order_relaxed);
    cv_.notify_all();
    for (auto& t : threads_) {
      t.join();
    }
    threads_.clear();
  }

private:
  static void initializeInstance() {
    if (instance_ == nullptr) {
      // 假定reset不会失败.
      instance_.reset(new ThreadPool());
    }
  }

  explicit ThreadPool(unsigned int num_thread = std::thread::hardware_concurrency())
      : action_thread_(0), should_terminate_(false) {
    threads_.reserve(num_thread);
    for (int i = 0; i < num_thread; ++i) {
      threads_.emplace_back(
          [this](std::string const& thread_name) {
            while (true) {
              task_type task;
              {
                std::unique_lock lock(mutex_);
                cv_.wait(lock, [this]() {
                  return should_terminate_.load(std::memory_order_relaxed) ||
                      !task_queue_.empty();
                });
                if (should_terminate_.load(std::memory_order_relaxed) &&
                    task_queue_.empty()) {
                  break;
                }
                task = std::move(task_queue_.front());
                task_queue_.pop();
              }

              ++action_thread_;
              task();
              --action_thread_;
            }
          },
          std::string("worker-").append(std::to_string(i)));
    }
  }
};

#endif // CPP20_ADVANCED_PROGRAMMING_THREAD_POOL_HPP