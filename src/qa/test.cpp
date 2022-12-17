/* Proj: Project
 * File: test.cpp
 * Created Date: 2022/12/2
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/12/2 21:55:57
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

/**
 * @file thread_pool.h
 * @author Sinter Wong (sintercver@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SIMPLE_THREAD_POOL_H_
#define __SIMPLE_THREAD_POOL_H_

/**
 * @brief 线程池是一个生产者消费者模型，由主线程生产任务，一堆子线程消费任务
 *
 */

#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class task {
public:
  task() = default;

  template <typename Func>
  task(Func&& f) : ptr_(new wrapper{std::move(f)}){};

  void operator()() { ptr_->call(); }

private:
  class wrapper_base {
  public:
    virtual void call() = 0;
    virtual ~wrapper_base() {}
  };

  template <typename Func>
  class wrapper : public wrapper_base {
  public:
    wrapper(Func&& func) : f_(std::move(func)) {}
    virtual void call() override { f_(); };

  private:
    Func f_;
  };

  std::unique_ptr<wrapper_base> ptr_;
};

class thread_pool {
public:
  thread_pool() : running(false) {}

  ~thread_pool() {
    // 如果用户用完没有调用stop()，那么线程没有释放就会报错，所以析构的时候还是手动调用一下stop()吧
    stop();
  }

  void start(size_t n) {
    if (running) {
      return;
    }
    running = true;
    threads_.reserve(n);
    while (n--) {
      threads_.emplace_back(&thread_pool::worker, this);
    }
  }

  void stop() {
    if (!running) {
      return;
    }
    {
      // 上锁，因为要释放了，所以需要所有线程停止工作
      std::lock_guard<std::mutex> lk(m_);
      running = false;

      not_empty_.notify_all();
      not_full_.notify_all();
    }

    // 释放掉所有的线程
    for (auto& t : threads_) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

  template <
      typename Func,
      typename... Args,
      typename Ret = std::invoke_result_t<Func, Args...>>
  std::future<Ret> submit(Func&& f, Args&&... args) {
    std::unique_lock<std::mutex> lk(m_);
    // not_full_.wait(lk, [this] { return !running || !q_.full(); });
    // if (!running) {
    //   return {};
    // }

    // std::packaged_task<Ret(Args...)> pt(std::forward<Func>(f));
    std::packaged_task<Ret()> pt(std::packaged_task<Ret()>(
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...)));
    auto ret = pt.get_future();

    task ptr(std::move(pt));
    q_.push(std::move(ptr));
    not_empty_.notify_one();

    return ret;
  }

private:
  // 生产者(submit)在外界不断给出任务，消费者(worker)需要不停的监视是否存在任务，因此需要设置无限循环
  void worker() {
    while (true) {
      task t;
      {
        std::unique_lock<std::mutex> lk(m_);
        not_empty_.wait(lk, [this] { return !running || !q_.empty(); });
        if (!running) {
          return;
        }
        t = std::move(q_.front());
        q_.pop();
        not_full_.notify_one();
      }
      // 函数的执行需要进行一段时间，这段时间内不需要上锁，因此需要释放掉
      // unique_lock
      t();
    }
  }

  std::vector<std::thread> threads_;

  // 任务队列需要选用长度固定的循环队列，防止溢出
  // circular_buffer<task, 8> q_;

  std::queue<task> q_;
  std::condition_variable not_full_; // 不未满条件
  std::condition_variable not_empty_; // 不为空条件
  std::mutex m_;

  bool running;
};

#endif

void f(const std::string& value) {
  std::cout << value << std::endl;
}

template <typename F, typename... Args, typename Ret = std::invoke_result_t<F, Args...>>
std::packaged_task<Ret()> wrap(F&& f, Args&&... args) {
  return std::packaged_task<Ret()>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}


template <typename T>
std::future<std::invoke_result_t<T>> test_lambda(T&& t) {
  std::promise<int> a;
  auto future = a.get_future();
  a.set_value(1);
  return future;
}


void test() {

  test_lambda([] { return 1; }).wait();

  // thread_pool tp;
  // tp.submit(f, std::move(data));
  std::string data = "hello world";
  // std::bind(f, std::move(data));
  // auto task = wrap(f, std::move(data));
  auto task = wrap(f, data);
}

int main() {
  test();
  return 0;
}