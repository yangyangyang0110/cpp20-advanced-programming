/* Proj: cpp20-advanced-programming
 * File: auto-ptr.cpp
 * Created Date: 2022/11/5
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/5 20:07:10
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <thread>
using namespace std::chrono_literals;

namespace detail {
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> dis(0, 15);

std::string genFakeString(unsigned int n = 4) noexcept {
  std::stringstream ss;
  for (auto i = 0; i < n; ++i)
    ss << dis(gen);
  return ss.str();
}

} // namespace detail

struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = default;
};

struct Redis {
  std::string username;
  std::string password;
  std::string hostname;

  Redis(std::string hostname, std::string username, std::string password)
      : username(std::move(username)),
        hostname(std::move(hostname)),
        password(std::move(password)) {}

  friend std::ostream& operator<<(std::ostream& os, const Redis& redis) {
    return os << redis.hostname << "(" << redis.username << ": "
              << redis.password << ")";
  }

  static Redis genFakeRedis() noexcept {
    return {
        detail::genFakeString(),
        detail::genFakeString(),
        detail::genFakeString()};
  }
};

class Connection {
 public:
  explicit Connection(Redis&& redis);
  void resetRedis(Redis&& redis) noexcept;

 private:
  std::shared_ptr<Redis> redis_;
};

class ConnectionProxy {
 public:
  static void setPtr(std::weak_ptr<Redis>&& ptr) noexcept;
  static void notify() noexcept;

 private:
  inline static std::weak_ptr<Redis> ptr_;
};

Connection::Connection(Redis&& redis)
    : redis_(std::make_shared<Redis>(std::move(redis))) {
  ConnectionProxy::setPtr(redis_);
}

void Connection::resetRedis(Redis&& redis) noexcept {
  redis_ = std::make_shared<Redis>(std::move(redis));
  ConnectionProxy::setPtr(redis_);
}

void ConnectionProxy::setPtr(std::weak_ptr<Redis>&& ptr) noexcept {
  ptr_ = std::move(ptr);
}

void ConnectionProxy::notify() noexcept {
  if (auto ptr = ptr_.lock()) {
    std::cout << *ptr << std::endl;
  } else {
    std::cerr << "Ptr is expired" << std::endl;
  }
}

void test() {
  auto conn = Connection(Redis::genFakeRedis());
  std::cout << "Connection" << std::endl;
  std::thread([]() {
    while (true) {
      std::this_thread::sleep_for(2s);
      ConnectionProxy::notify();
    }
  }).detach();

  std::thread([&conn]() {
    while (true) {
      std::this_thread::sleep_for(4s);
      conn.resetRedis(Redis::genFakeRedis());
    }
  }).join();
}

int main() {
  test();
  return 0;
}
