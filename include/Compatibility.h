//
// Created by liuhanyu on 2021/4/7.
// Compatibility header

#ifndef NACOS_SDK_CPP_COMPATIBILITY_H
#define NACOS_SDK_CPP_COMPATIBILITY_H

#if __cplusplus >= 201402L
//C++ 17 compatibility
#define NACOS_THROW(...)
#define NACOS_NOTHROW() noexcept
#else
#define NACOS_THROW throw
#define NACOS_NOTHROW throw
#endif

#endif //NACOS_SDK_CPP_COMPATIBILITY_H
