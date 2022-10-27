/* Proj: cpp20-advanced-programming
 * File: visitor-pattern.cpp
 * Created Date: 2022/10/19
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/19 08:39:20
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */
#include <iostream>

/**
 * @brief 观察者模式
 */

struct TextFile;
struct VideoFile;

struct Visitor {
    virtual void visit(VideoFile &) const noexcept = 0;
    virtual void visit(TextFile &) const noexcept = 0;
    virtual ~Visitor() = default;
};

struct Element {
    virtual void accept(Visitor &) = 0;
    virtual ~Element() = default;
};
#if 1
template <typename Derived> struct AutoDispatchElement : public Element {
    void accept(Visitor &visitor) override { visitor.visit(static_cast<Derived &>(*this)); }
};

struct VideoFile : AutoDispatchElement<VideoFile> {
};

struct TextFile : AutoDispatchElement<VideoFile> {
};
#else

struct VideoFile : Element {
    void accept(Visitor &visitor) override { visitor.visit(*this); }
};

struct TextFile : Element {
    void accept(Visitor &visitor) override { visitor.visit(*this); }
};
#endif

void test() {}

int main()
{
    test();
    return 0;
}
