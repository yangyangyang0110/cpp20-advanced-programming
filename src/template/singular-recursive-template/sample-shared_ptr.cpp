/* Proj: cpp20-advanced-programming
 * File: sample-shared_ptr.cpp
 * Created Date: 2022/10/19
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/19 09:54:56
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

/**
 * @brief this to shared ptr.
 */

struct Handler : std::enable_shared_from_this<Handler> {

    void handle() const noexcept { std::cout << "Handler: " << message << std::endl; }

    std::weak_ptr<Handler> getSharedPtr() { return weak_from_this(); }

    void async_callback()
    {
        [wp = this->weak_from_this()]() {
            if (auto sp = wp.lock()) {
                sp->handle();
            } else {
                std::cerr << "Pointer is null" << std::endl;
            }
        }();
    }

    // No public constructor, only a factory function,
    // so there's no way to have getptr return nullptr.
    //     [[nodiscard]] static std::shared_ptr<Handler> create()
    //     {
    //         // Not using std::make_shared<Best> because the c'tor is private.
    //         return std::shared_ptr<Handler>(new Handler());
    //     }
    //
    // private:
    //     Handler() = default;
    std::string message = "I'm a Message.";
};

void timeoutFunction(std::weak_ptr<Handler> ctx)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    if (auto p = ctx.lock()) {
        p->handle();
    } else {
        std::cerr << "Pointer is null" << std::endl;
    }
}

void timeoutFunctionUnsafePointer(Handler *&p)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    if (p) {
        p->handle();
    } else {
        std::cerr << "Pointer is null" << std::endl;
    }
}

void test()
{
    // auto p = Handler().getSharedPtr();
    auto h = Handler();
    auto p = h.getSharedPtr();
    std::thread(timeoutFunction, p).detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Maybe memory unsafe.
    // auto *p = new Handler;
    // auto pr = p.get();
    // std::thread(timeoutFunctionUnsafePointer, std::ref(pr)).detach();
}

int main()
{
    test();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
