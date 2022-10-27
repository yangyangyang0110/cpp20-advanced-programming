/* Proj: cpp20-advanced-programming
 * File: like-rust-result.cpp
 * Created Date: 2022/10/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/20 22:40:33
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>

template <typename T, typename E> class Result
{
public:
    static Result<T, E> success(T &&t) { return Result<T, E>(std::move(t)); }

    static Result<T, E> error(E &&e) { return Result<T, E>(std::move(e)); }

    [[nodiscard]] const T &value() const noexcept { return value_; }
    T &value() noexcept { return value_; }

    [[nodiscard]] const E &error() const noexcept { return error_; }
    E &error() noexcept { return error_; }

private:
    T value_;
    E error_;
    bool is_valid_value = false;
    Result() = default;

    explicit Result(T &&t)
        : value_(std::move(t))
        , is_valid_value(true){

          };

    explicit Result(E &&e)
        : error_(std::move(e))
        , is_valid_value(false){

          };
};

struct Foo {
    static Result<Foo, std::string> tryFrom(bool b)
    {
        return b ? Result<Foo, std::string>::success(Foo())
                 : Result<Foo, std::string>::error("failed to create");
    }

    Foo() = default;

    friend std::ostream &operator<<(std::ostream &os, const Foo &foo) { return os << "Foo"; }
};

void test() { std::cout << Foo::tryFrom(true).value() << ";" << Foo::tryFrom(false).error(); }

int main()
{
    test();
    return 0;
}
