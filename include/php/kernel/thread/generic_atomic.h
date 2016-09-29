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
#ifndef PHP_KERNEL_THREAD_GENERIC_ATOMIC_H
#define PHP_KERNEL_THREAD_GENERIC_ATOMIC_H

#include <cstddef>

#include "php/kernel/base/type_info.h"

namespace Php {
namespace Kernel {
namespace Thread {

template<int> 
struct AtomicOperationsSupport
{
   PHP_DECL_CONSTEXPR int IsSupported = 0;
};

template<>
struct AtomicOperationsSupport<4>
{
   PHP_DECL_CONSTEXPR int IsSupported = 1;
};

template <typename T> 
struct AtomicAdditiveType
{
   using AdditiveType = T;
   static const int addScale = 1;
};

template <typename T>
struct AtomicAdditiveType<T *>
{
   using AdditiveType = std::ptrdiff_t;
   static const int addScale = sizeof(T);
};

/*
 * @TODO 使用concept优化 
 */
template <typename BaseClass>
struct GenericAtomOperations
{
   template <typename T>
   struct AtomicType
   {
      using Type = T;
      using PointerType = T*;
   };
   
   template <typename T>
   static void acquireMemoryFence(const T &value) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
   }
   
   template <typename T>
   static void releaseMemoryFence(const T &value) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
   }
   
   
};

} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_GENERIC_ATOMIC_H
