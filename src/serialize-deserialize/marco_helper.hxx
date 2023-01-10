/*
 * File: marco_helper.hpp
 * Created Date: 2023/1/10
 * -----
 * Copyright (c) 2023  . All rights reserved.
 */
#ifndef MARCO_HELPER_HPP
#define MARCO_HELPER_HPP

#define EXPAND(x) x

#define GET_MARCO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, M, ...) M

#define PASTE(f, ARGS...) \
  GET_MARCO(ARGS, P16, P15, P14, P13, P12, P11, P10, P9, P8, P7, P6, P5, P4, P3, P2, P1)(f, ARGS)

#define P1(f, v0)                                 f(v0)
#define P2(f, v0, v1)                             P1(f, v0) P1(f, v1)
#define P3(f, v0, v1, v2)                         P1(f, v0) P2(f, v1, v2)
#define P4(f, v0, v1, v2, v3)                     P1(f, v0) P3(f, v1, v2, v3)
#define P5(f, v0, v1, v2, v3, v4)                 P1(f, v0) P4(f, v1, v2, v3, v4)
#define P6(f, v0, v1, v2, v3, v4, v5)             P1(f, v0) P5(f, v1, v2, v3, v4, v5)
#define P7(f, v0, v1, v2, v3, v4, v5, v6)         P1(f, v0) P6(f, v1, v2, v3, v4, v5, v6)
#define P8(f, v0, v1, v2, v3, v4, v5, v6, v7)     P1(f, v0) P7(f, v1, v2, v3, v4, v5, v6, v7)
#define P9(f, v0, v1, v2, v3, v4, v5, v6, v7, v8) P1(f, v0) P8(f, v1, v2, v3, v4, v5, v6, v7, v8)
#define P10(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
  P1(f, v0) P9(f, v1, v2, v3, v4, v5, v6, v7, v8, v9)
#define P11(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
  P1(f, v0) P10(f, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define P12(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
  P1(f, v0) P11(f, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define P13(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
  P1(f, v0) P12(f, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define P14(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) \
  P1(f, v0) P13(f, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define P15(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14) \
  P1(f, v0) P14(f, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define P16(f, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) \
  P1(f, v0) P15(f, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)

#endif // MARCO_HELPER_HPP
