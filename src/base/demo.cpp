/* Proj: cpp20-advanced-programming
 * File: demo.cpp
 * Created Date: 2022/11/23
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/23 22:43:47
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std::chrono_literals;

class ThreadPool {
  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> task_queue_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cv_;
  std::atomic<bool> should_terminate_;
  std::atomic<int32_t> action_thread_;

public:
  ~ThreadPool() {
    should_terminate_.store(true, std::memory_order_relaxed);
    cv_.notify_all();
    for (auto& t : threads_) {
      t.join();
    }
    threads_.clear();
  }

  explicit ThreadPool(unsigned int num_thread = std::thread::hardware_concurrency())
      : action_thread_(0) {
    should_terminate_.store(false, std::memory_order_relaxed);
    threads_.reserve(num_thread);
    for (int i = 0; i < num_thread; ++i) {
      threads_.emplace_back(std::thread(
        [this](std::string const& thread_name) {
          while (true) {
            std::function<void(void)> task;
            {
              std::unique_lock lock(mutex_);
              cv_.wait(lock, [this]() {
                return should_terminate_.load(std::memory_order_relaxed) ||
                  !task_queue_.empty();
              });

              if (
                should_terminate_.load(std::memory_order_relaxed) &&
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
        std::string("worker-").append(std::to_string(i))));
    }
  }

  template <
    typename Func,
    typename... Args,
    typename RetType = std::invoke_result_t<Func, Args...>>
  std::future<RetType> execute(Func&& func, Args&&... args) {
    std::packaged_task<RetType(void)> task(
      std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    auto fut = task.get_future();

    std::lock_guard lock(mutex_);
    // move capture. c++14
    // task_queue_.emplace([task = std::move(task)]() mutable { task(); });
    task_queue_.emplace([taskPtr = std::make_shared<decltype(task)>(
                           std::move(task))]() mutable { (*taskPtr)(); });
    cv_.notify_all();
    return fut;
  }
};

void test() {
  using std::future;
  using std::vector;

  ThreadPool tp(2);
  vector<future<int>> res;
  for (int i = 0; i < 10; ++i) {
    res.push_back(tp.execute(
      [](int j) {
        // std::cout << "value: " << j * 100 << std::endl;
        return j * 100;
      },
      i + 10));
  }

  // mock io task.
  tp.execute([] {
    // sleep some time.
    std::this_thread::sleep_for(5s);
    std::cout << "Io task Done." << std::endl;
  });

  for (auto& item : res) {
    std::cout << "Future get " << item.get() << std::endl;
  }

  std::cout << "Function end." << std::endl;
  // std::this_thread::sleep_for(8s);
}

int main() {
  test();
  return 0;
}