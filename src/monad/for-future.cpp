/* Proj: cpp-functional-programming
 * File: for-future.cpp
 * Created Date: 2022/11/12
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/11/12 19:18:30
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <boost/thread/future.hpp>

using namespace boost;

// (future<T1>, f: -> future<T2>) => future<T2>
template <typename T, typename F>
auto m_bind(future<T>& fut, const F& f) {
  return fut.then(boost::launch::deferred, [&](future<T> finish) {
    // std::cout << "Hello" << std::endl;
    return f(finish.get());
  });
}

void test() {
  future<int> f = async([]() {
    // 读数据库.
    return 123;
  });

  future<int> f2 = m_bind(f, [](const int& v) { return v + 10; });
  // std::cout << f2.get() << std::endl;
}

int main() {
  test();
  return 0;
}
