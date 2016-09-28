/**
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: zzu_softboy <zzu_softboy@163.com>                           |
   +----------------------------------------------------------------------+
*/
#ifndef PHP_KERNEL_BASE_GLOBAL_H
#define PHP_KERNEL_BASE_GLOBAL_H

#include "php_config.h"
#ifdef __cplusplus
#  include <cstddef>
#endif

#include <stddef.h>

#define PHP_VERSION_CHECK(major, minor, patch) ((major << 16) | (minor << 8) | patch)
#define PHP_VERSION PHP_VERSION_CHECK(PHP_VERSION_MAJOR, PHP_VERSION_MINOR, PHP_VERSION_PATCH)

/*
    The PHP_CONFIG macro implements a safe compile time check for fe  datures of php++.
    Features can be in three states:
        0 or undefined: This will lead to a compile error when testing for it
        -1: The feature is not available
        1: The feature is available
*/
#define PHP_CONFIG(feature) (1/PHP_FEATURE_##feature == 1)

/* These two macros makes it possible to turn the builtin line expander into a
 * string literal. */
#define PHP_STRINGIFY2(str) #str
#define PHP_STRINGIFY(str) PHP_STRINGIFY2(str)

#include "php/kernel/base/system_detection.h"
#include "php/kernel/base/processor_detection.h"
#include "php/kernel/base/compiler_detection.h"

#if defined(__EFI__)
#  define PHP_OF_ELF
#endif

#if defined(__MACH__) && defined(__APPLE__)
#  define PHP_OF_MACH_O
#endif

#if __cplusplus
#  include <algorithm>

#  define PHP_PREPEND_G_NAMESPACE(name) ::php::name
#  define PHP_USE_G_NAMESPACE using namespace ::php;
#  define PHP_BEGIN_G_NAMESPACE namespace php{
#  define PHP_END_NAMESPACE }
#  define PHP_EXIT_G_NAMESPACE }
#  define PHP_ENTER_G_NAMESPACE namespace php{

namespace php{} //定义全局的php名称空间

#endif // __cplusplus

PHP_BEGIN_G_NAMESPACE
// Size-dependent types (architechture-dependent byte order)
typedef signed char php_int8; // 8 bit signed
typedef unsigned char php_uint8; // 8 bit unsigned
typedef short php_int16; // 16 bit signed
typedef unsigned short php_uint16; // 16 bit unsigned
typedef int php_int32; // 32 bit signed
typedef unsigned int php_uint32; // 32 bit unsigned

#if defined(PHP_OS_WIN) && !defined(PHP_CC_GNU)
#  define PHP_INT64_C(c) c ## i64 // signed 64 bit constant
#  define PHP_UINT64_C(c) c ## ui64 // 64 bit unsigned
typedef __int64 php_int64; // 64 bit signed
typedef unsigned __int64 php_uint64; // 64 bit unsigned
#else
#  define PHP_INT64_C(c) static_cast<long long>(c ## LL) // signed 64 bit constant
#  define PHP_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) // unsigned 64 bit constant
typedef long long php_int64; // 64 bit signed
typedef unsigned long long php_uint64; // 64 bit unsigned
#endif

typedef php_int64 php_longlong;
typedef php_uint64 php_ulonglong;

// useful type definitions for php
PHP_EXIT_G_NAMESPACE

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

PHP_ENTER_G_NAMESPACE

#if defined(PHP_NO_DEPRECATED)
#  undef PHP_DEPRECATED
#  undef PHP_DEPRECATED_X
#  undef PHP_DEPRECATED_VARIABLE
#  undef PHP_DEPRECATED_CONSTRUCTOR
#elif defined(PHP_DEPRECATED_WARNINGS)
#  undef PHP_DEPRECATED
#  define PHP_DEPRECATED PHP_DECL_DEPRECATED
#  undef PHP_DEPRECATED_X
#  define PHP_DEPRECATED_X(text) PHP_DECL_DEPRECATED_X(text)
#  undef PHP_DEPRECATED_VARIABLE
#  define PHP_DEPRECATED_VARIABLE PHP_DECL_VARIABLE_DEPRECATED
#  else
#  undef PHP_DEPRECATED
#  define PHP_DEPRECATED
#  undef PHP_DEPRECATED_X
#  define PHP_DEPRECATED_X(text)
#  undef PHP_DEPRECATED_VARIABLE
#  define PHP_DEPRECATED_VARIABLE
#  undef PHP_DEPRECATED_CONSTRUCTOR
#endif

#ifndef PHP_DISABLE_DEPRECATED_BEFORE
#define PHP_DISABLE_DEPRECATED_BEFORE PHP_VERSION_CHECK(1, 0, 0)
#endif

#ifdef PHP_DEPRECATED
#  define PHP_DEPRECATED_SINCE(major, minor) (PHP_VERSION_CHECK(major, minor, 0) > PHP_DISABLE_DEPRECATED_BEFORE)
#else
#  define PHP_DEPRECATED_SINCE(major, minor) 0
#endif

#ifdef PHP_BUILD_SHARED_LIBS
#  define PHP_CORE_EXPORT PHP_DECL_EXPORT
#else
#  define PHP_CORE_EXPORT
#endif

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define PHP_DISABLE_COPY(Class) \
   Class(const Class &) PHP_DECL_EQ_DELETE;\
   Class &operator=(const Class &) PHP_DECL_EQ_DELETE



// 定义一些常用的宏函数

template<typename T>
PHP_DECL_CONSTEXPR inline T php_abs(const T &t) 
{
   return t >= 0 ? t : -t;
}
/*
 * 这几个函数到时候可以使用最新的concept进行优化
 */
PHP_DECL_CONSTEXPR inline php_round(double d)
{
   return d >= 0.0 ? int(d + 0.5) : int(d - double(int(d - 1)) + 0.5) + int(d - 1);
}

PHP_DECL_CONSTEXPR inline php_round(float f)
{
   return f > 0.0f ? int(f + 0.5f) : int(f - float(int(f - 1)) + 0.5f) + int(f - 1);
}

PHP_DECL_CONSTEXPR inline php_round64(double d)
{
   return d >= 0.0 ? php_int64(d + 0.5) : php_int64(d - double(php_int64(d - 1)) + 0.5) + php_int64(d - 1);
}

PHP_DECL_CONSTEXPR inline php_round64(float f)
{
   return f > 0.0f ? php_int64(f + 0.5f) : php_int64(f - float(php_int64(f - 1)) + 0.5f) + php_int64(f - 1);
}

template <typename T>
PHP_DECL_CONSTEXPR inline T &php_min(const T &left, const T &right)
{
   return (left < right) ? left : right;
}

template <typename T>
PHP_DECL_CONSTEXPR inline T &php_max(const T &left, const T &right)
{
   return (left > right) ? left : right;
}

template <typename T>
PHP_DECL_CONSTEXPR inline T &php_bound(const T &min, const T &value, const T &max)
{
   return php_max(min, php_min(value, max));
}

PHP_END_NAMESPACE

#endif // PHP_KERNEL_BASE_GLOBAL_H