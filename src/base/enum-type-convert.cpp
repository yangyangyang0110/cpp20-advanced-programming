/* Proj: cpp20-advanced-programming
 * File: enum-type-convert.cpp
 * Created Date: 2022/10/20
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/20 15:35:37
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <cassert>
#include <iostream>

#ifdef __DEBUG__
using ASSERT = assert;
#else
#define ASSERT(x)                                                                                  \
    if (!(x)) throw std::runtime_error("Assertion failed: " #x);
#endif

/**
 * @brief 如何优化的转换不同Enum类型, 以及对于未处理的新枚举值会显示提示给用户(报错等方式)
 */

enum class RequestCode {
    Ok            = 0,
    InvalidFile   = 1,
    InvalidMethod = 2,
    NewCode       = 100,
};

enum class ResponseCode {
    Ok                   = -1,
    InvalidRequestMethod = 1,
    InvalidFile          = 2,
    PlaceHolderCode      = -100,
};

/**
 * @brief 1. 自由函数,
 *  问题: 繁琐, 不好维护(尤其是新增了code后, 需要手动找到此函数添加,
 *  否则抛出运行期错误, TODO: 如何编译期报错)
 */

ResponseCode getResponseCode(const RequestCode &code) noexcept(false)
{
    if (code == RequestCode::Ok) return ResponseCode::Ok;
    else if (code == RequestCode::InvalidMethod) return ResponseCode::InvalidRequestMethod;
    else if (code == RequestCode::InvalidFile) return ResponseCode::InvalidFile;
    throw std::bad_cast();
}

void test_by_function()
{
    const auto code = RequestCode::Ok;
    ASSERT(ResponseCode::Ok == getResponseCode(code))
    try {
        ASSERT(ResponseCode::PlaceHolderCode == getResponseCode(code))
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        ASSERT(ResponseCode::PlaceHolderCode == getResponseCode(RequestCode::NewCode))
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

/**
 * @brief 2. 模式匹配(此处条件未知, 使用标签分发代替`if constexpr`), 编译器报错
 * 问题： 错误信息不友好, TODO: 如何使用`enable_if`做约束
 */

namespace detail::m2 {

template <RequestCode> struct always_false : public std::false_type {
};

template <RequestCode code> struct ToResponseCode {
    // TODO: 求值上下文中报错信息不友好
    static_assert(always_false<code>::value, "Assertion failed, Enum value can't matched.");
};

template <> struct ToResponseCode<RequestCode::Ok> {
    static constexpr ResponseCode value = ResponseCode::Ok;
};

template <> struct ToResponseCode<RequestCode::InvalidFile> {
    static constexpr ResponseCode value = ResponseCode::InvalidFile;
};

template <> struct ToResponseCode<RequestCode::InvalidMethod> {
    static constexpr ResponseCode value = ResponseCode::InvalidRequestMethod;
};

} // namespace detail::m2

template <RequestCode code> static constexpr ResponseCode getResponseCodeValue =
    detail::m2::ToResponseCode<code>::value;

void test_by_pattern_match()
{
    const auto code = RequestCode::Ok;
    ASSERT(ResponseCode::Ok == getResponseCodeValue<code>)
    try {
        ASSERT(ResponseCode::PlaceHolderCode == getResponseCodeValue<code>)
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // ASSERT(ResponseCode::PlaceHolderCode == getResponseCodeValue<RequestCode::NewCode>)
}

/**
 * @brief 编译期Enum不支持的转换报错, 并且报错信息友好到调用端(通过参数约束)
 */

namespace detail::m3 {

template <RequestCode code> struct ToResponseCode : public std::integral_constant<void *, nullptr> {
};

template <> struct ToResponseCode<RequestCode::Ok>
    : public std::integral_constant<ResponseCode, ResponseCode::Ok> {
};

template <> struct ToResponseCode<RequestCode::InvalidFile>
    : public std::integral_constant<ResponseCode, ResponseCode::InvalidFile> {
};

template <> struct ToResponseCode<RequestCode::InvalidMethod>
    : public std::integral_constant<ResponseCode, ResponseCode::InvalidRequestMethod> {
};

template <RequestCode code> struct ToResponseCodeProxy : public ToResponseCode<code> {
};

} // namespace detail::m3

template <
    RequestCode code,
    std::enable_if_t<std::is_same_v<
        ResponseCode, std::remove_cvref_t<decltype(detail::m3::ToResponseCode<code>::value)>>>>
ResponseCode getResponseCodeValueM3Func()
{
    return detail::m3::ToResponseCodeProxy<code>::value;
}

template <RequestCode code,
          std::enable_if_t<
              std::is_same_v<ResponseCode, std::remove_cvref_t<
                                               decltype(detail::m3::ToResponseCode<code>::value)>>,
              void *> = nullptr>
static constexpr ResponseCode getResponseCodeValueM3 = detail::m3::ToResponseCodeProxy<code>::value;

void test_by_pattern_match_friendly_error()
{
    constexpr auto code = RequestCode::Ok;
    ASSERT(ResponseCode::Ok == getResponseCodeValueM3<code>)
    try {
        ASSERT(ResponseCode::PlaceHolderCode == getResponseCodeValueM3<code>)
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    constexpr auto newCode = RequestCode::Ok;

    // ASSERT(ResponseCode::PlaceHolderCode == getResponseCodeValueM3<newCode>)
    // ASSERT(ResponseCode::PlaceHolderCode == getResponseCodeValueM3Func<newCode>())
}

void test()
{
    test_by_function();
    test_by_pattern_match();
    test_by_pattern_match_friendly_error();
}

int main()
{
    test();
    return 0;
}