/**
 * @file 03_non_blocking_io_by_callback.cpp
 * @author
 * @brief
 * @version 0.1
 * @date 2022-08-27
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <coroutine>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

namespace fs = std::filesystem;

namespace blocking {

std::optional<std::vector<std::string>> file2vector(const fs::path& file_path) {
  if (!fs::exists(file_path)) {
    fprintf(stderr, "Assertion file %s isn't exists.\n", file_path.c_str());
    return std::nullopt;
  }

  std::ifstream file(file_path.c_str());
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open file %s.\n", file_path.c_str());
    return std::nullopt;
  }

  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.emplace_back(std::move(line));
  }
  file.close();
  return lines;
}

} // namespace blocking

namespace non_blocking_by_callback {

using callback_t = void(std::string&& line);

bool file2vector(const fs::path& file_path, callback_t cb) {
  if (!fs::exists(file_path)) {
    fprintf(stderr, "Assertion file %s isn't exists.\n", file_path.c_str());
    return false;
  }

  std::ifstream file(file_path.c_str());
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open file %s.\n", file_path.c_str());
    return false;
  }

  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    cb(std::move(line));
  }
  file.close();
  return true;
}

} // namespace non_blocking_by_callback

namespace non_blocking_by_coroutine {

template <typename T>
struct Generator {
  struct promise_type {
    const T& result() const noexcept { return value_; }

    template <std::convertible_to<T> From> // C++20 concept
    std::suspend_always yield_value(From&& from) noexcept {
      value_ = std::forward<From>(from); // caching the result in promise
      return {};
    }

    void return_void() noexcept {}

    std::suspend_always initial_suspend() noexcept { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    void unhandled_exception() {
      exception_ = std::current_exception();
    } // saving exception

    Generator get_return_object() noexcept {
      return Generator{
          std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::exception_ptr exception_;

   private:
    T value_;
  };

  // 值传递.
  explicit Generator(std::coroutine_handle<promise_type> handle) noexcept
      : handle_(handle) {}

  ~Generator() {
    std::cout << "~Generator\n";
    if (handle_) {
      std::cout << "handle_.destroy()\n";
      handle_.destroy();
    }
  }

  explicit operator bool() {
    fill(); // The only way to reliably find out whether or not we finished
            // coroutine, whether or not there is going to be a next value
            // generated (co_yield) in coroutine via C++ getter (operator ()
            // below) is to execute/resume coroutine until the next co_yield
            // point (or let it fall off end). Then we store/cache result in
            // promise to allow getter (operator() below to grab it without
            // executing coroutine).
    return !handle_.done();
  }

  const T& operator()() noexcept {
    fill();
    full_ = false; // we are going to move out previously cached
                   // result to make promise empty again
    return handle_.promise().result();
  }

 private:
  std::coroutine_handle<promise_type> handle_;
  bool full_ = false;

  void fill() {
    if (!full_) {
      handle_();
      if (handle_.promise().exception_)
        std::rethrow_exception(handle_.promise().exception_);
      // propagate coroutine exception in called context
      full_ = true;
    }
  }
};

Generator<std::string> file2vector(const fs::path& file_path) {
  if (!fs::exists(file_path)) {
    fprintf(stderr, "Assertion file %s isn't exists.\n", file_path.c_str());
    co_return;
  }

  std::ifstream file(file_path.c_str());
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open file %s.\n", file_path.c_str());
    co_return;
  }

  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line))
    co_yield std::move(line);
  file.close();
  co_return;
}

} // namespace non_blocking_by_coroutine

void test_blocking(const fs::path& path) {
  auto datas = blocking::file2vector(path);
  if (datas.has_value())
    for (const auto& line : datas.value())
      std::cout << line << std::endl;
}

void test_non_blocking_with_callback(const fs::path& path) {
  const bool flag = non_blocking_by_callback::file2vector(
      path, [](std::string&& line) { std::cout << line << std::endl; });
}

void test_non_blocking_with_coroutine(const fs::path& path) {
  auto g = non_blocking_by_coroutine::file2vector(path);
  for (; g;)
    std::cout << g() << std::endl;
  std::cout << "\n\n\nDone\n\n\n" << std::endl;
}

int main() {
  const fs::path txt_path{
      "/Users/yang/Code/cpp20-advanced-programming/data/text/random.txt"};
  // test_blocking(txt_path);
  // test_non_blocking_with_callback(txt_path);
  test_non_blocking_with_coroutine(txt_path);
  return 0;
}
