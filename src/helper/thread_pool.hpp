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

namespace thread_pool {

namespace _utils {

#if __cplusplus <= 201402L
template <
    typename Func,
    typename... Args,
    typename RetType = typename std::result_of<Func(Args...)>::type>
inline auto make_task(Func&& func, Args&&... args)
    -> std::packaged_task<RetType(void)> {
#else
template <
    typename Func,
    typename... Args,
    typename RetType = std::result_of_t<Func(Args...)>>
inline auto make_task(Func&& func, Args&&... args)
    -> std::packaged_task<RetType(void)> {
#endif
  return std::packaged_task<RetType(void)>(
      std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
}

} // namespace _utils

class ThreadPool {
  /**
   * support get function return value.
   * TODO: worker extract to outside.
   */
  std::atomic_bool is_terminal_;
  std::vector<std::thread> workers_;
  thread_safe_queue<std::packaged_task<void(void)>> tasks_;

public:
  explicit ThreadPool(size_t n = std::thread::hardware_concurrency())
      : is_terminal_(false) {
    workers_.reserve(n);
    for (auto i = 0; i < n; ++i)
      workers_.emplace_back(std::thread(
          [this](const std::string& id) {
            while (!is_terminal_) {
              decltype(tasks_)::value_type task;
              if (tasks_.try_pop(task)) {
                INFO("[{}] got a job, executing...", id);
                task();
              }
            }
          },
          fmt::format("worker_{}", i)));
  }

  ~ThreadPool() {
    is_terminal_.store(true);
    for (auto& worker : workers_)
      if (worker.joinable())
        worker.join();
  }

  template <
      typename Function,
      typename RetType = typename std::result_of<Function(void)>::type>
  auto execute(Function f) -> std::future<RetType> {
    std::packaged_task<RetType()> task(std::move(f));
    auto res = task.get_future();
#if __cplusplus >= 201402L
    // move semantic lambda. c++14.
    auto payload = [task = std::move(task)]() mutable -> void { task(); };
#else
    // todo: optimizer.
    auto task_ptr = std::make_shared<decltype(task)>(std::move(task));
    auto payload = [task_ptr]() { task_ptr->operator()(); };
    // std::packaged_task<void(void)> payload = std::bind(
    //     [](const std::unique_ptr<decltype(task)> &task_ptr) -> void {
    //     task_ptr-> ; }, std::move(task));
#endif
    tasks_.emplace(decltype(tasks_)::value_type(std::move(payload))); // cast.
    return res;
  }
};

} // namespace thread_pool

#endif // CPP20_ADVANCED_PROGRAMMING_THREAD_POOL_HPP
