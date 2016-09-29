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
#ifndef PHP_KERNEL_BASE_TYPE_INFO_H
#define PHP_KERNEL_BASE_TYPE_INFO_H

#include <type_traits>
#include "php/kernel/base/global.h"

namespace Php {
namespace Kernel {
namespace Base {

/*
   Specialize a specific type with:
     PHP_DECLARE_TYPEINFO(type, flags);
   where 'type' is the name of the type to specialize and 'flags' is
   logically-OR'ed combination of the flags below.
*/

PHP_DECL_CONSTEXPR int TYPE_FLAG_COMPLEX_TYPE = 0;
PHP_DECL_CONSTEXPR int TYPE_FLAG_PRIMITIVE_TYPE = 0x1;
PHP_DECL_CONSTEXPR int TYPE_FLAG_STATIC_TYPE = 0;
PHP_DECL_CONSTEXPR int TYPE_FLAG_MOVABLE_TYPE = 0x2;
PHP_DECL_CONSTEXPR int TYPE_FLAG_DUMMY_TYPE = 0x4;
PHP_DECL_CONSTEXPR int TYPE_FLAG_RELOCATABLE_TYPE = 0x8;

/*
  定义最基本的TypeInfo模板 
 */
template <typename T>
class TypeInfo
{
public:
   enum {
      isPointer = false,
      isIntegral = std::is_integral<T>::value,
      isComplex = true,
      isStatic = true,
      isRelocatable = std::is_enum<T>::value,
      isLarge = (sizeof(T) > sizeof(void*)),
      sizeOf = sizeof(T)
   };
};

/*
   针对void的特化
 */
template<>
class TypeInfo<void>
{
public:
   enum {
      isPointer = false,
      isIntegral = false,
      isComplex = false,
      isStatic = false,
      isRelocatable = false,
      isLarge = false,
      sizeOf = 0
   };
};

/*
 *  针对不同指针的偏特化
 */
template<typename T>
class TypeInfo<T*>
{
public:
   enum {
      isPointer = true,
      isIntegral = false,
      isComplex = false,
      isStatic = false,
      isRelocatable = true,
      isLarge = false,
      sizeOf = sizeOf(T*)
   };
};

template <typename T, typename T1, typename T2 = T1, typename T3 = T1, typename T4 = T1>
class TypeInfoMerger
{
public:
   enum {
      isComplex = TypeInfoMerger<T1>::isComplex || TypeInfoMerger<T2>::isComplex ||
      TypeInfoMerger<T3>::isComplex || TypeInfoMerger<T4>::isComplex,
      isStatic = TypeInfoMerger<T1>::isStatic || TypeInfoMerger<T2>::isStatic ||
      TypeInfoMerger<T3>::isStatic || TypeInfoMerger<T4>::isStatic,
      isRelocatable = TypeInfoMerger<T1>::isRelocatable || TypeInfoMerger<T2>::isRelocatable ||
      TypeInfoMerger<T3>::isRelocatable || TypeInfoMerger<T4>::isRelocatable,
      isLarge = sizeof(T) > sizeof(void*),
      isPointer = false,
      isIntegral = false,
      isDummy = false,
      sizeOf = sizeof(T)
   };
};

#define PHP_DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class TypeInfo< TYPE > \
{ \
public:\
   enum { \
      isComplex = (((FLAGS) & TYPE_FLAG_COMPLEX_TYPE) == 0), \
      isStatic = (((FLAGS) & (TYPE_FLAG_MOVABLE_TYPE | TYPE_FLAG_COMPLEX_TYPE)) == 0), \
      isRelocatable = !isStatic || ((FLAGS) & TYPE_FLAG_RELOCATABLE_TYPE), \
      isLarge = (sizeof(TYPE)>sizeof(void*)), \
      isPointer = false, \
      isIntegral = std::is_integral< TYPE >::value, \
      isDummy = (((FLAGS) & TYPE_FLAG_DUMMY_TYPE) != 0), \
      sizeOf = sizeof(TYPE) \
   }; \
   static inline const char *name() { return #TYPE; } \
}

#define PHP_DECLARE_TYPEINFO(TYPE, FLAGS) \
template <> \ 
PHP_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)

/*
   Specialize a shared type with:
     PHP_DECLARE_SHARED(type)
*/
#define PHP_DECLARE_SHARED_IMPL(TYPE, FLAGS) \
PHP_DECLARE_TYPEINFO(TYPE, FLAGS);\
inline void swap(TYPE &value1, TYPE &value2)\
   PHP_DECL_NOEXCEPT_EXPR(noexcept(value1.swap(value2))) \
{ \
   value1.swap(value2);\
}

#define PHP_DECLARE_SHARED(TYPE) PHP_DECLARE_SHARED_IMPL(TYPE, TYPE_FLAG_MOVABLE_TYPE)

/*
   TypeInfo primitive specializations
*/
PHP_DECLARE_TYPEINFO(bool, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(char, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(signed char, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(uchar, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(short, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(ushort, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(int, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(uint, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(long, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(ulong, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(Php::php_int64, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(Php::php_uint64, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(float, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(double, TYPE_FLAG_PRIMITIVE_TYPE);
#ifndef PHP_OS_DARWIN
PHP_DECLARE_TYPEINFO(long double, TYPE_FLAG_PRIMITIVE_TYPE);
#endif

#ifdef PHP_COMPILER_UNICODE_STRINGS
PHP_DECLARE_TYPEINFO(char16_t, TYPE_FLAG_PRIMITIVE_TYPE);
PHP_DECLARE_TYPEINFO(char32_t, TYPE_FLAG_PRIMITIVE_TYPE);
#  if !defined(PHP_CC_MSVC) || defined(_NATIVE_WCHAR_T_DEFINED)
PHP_DECLARE_TYPEINFO(wchar_t, TYPE_FLAG_PRIMITIVE_TYPE);
#  endif
#endif



} // Base
} // Kernel
} // Php

#endif // PHP_KERNEL_BASE_TYPE_INFO_H
