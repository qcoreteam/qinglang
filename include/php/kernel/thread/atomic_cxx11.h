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
#ifndef PHP_KERNEL_THREAD_ATOMIC_CXX11_H
#define PHP_KERNEL_THREAD_ATOMIC_CXX11_H

#include <atomic>

#include "php/kernel/thread/generic_atomic.h"

namespace Php {
namespace Kernel {
namespace Thread {

/* Attempt to detect whether the atomic operations exist in hardware
 * or whether they are emulated by way of a lock.
 *
 * C++11 29.4 [atomics.lockfree] p1 says
 *
 *  The ATOMIC_..._LOCK_FREE macros indicate the lock-free property of the
 *  corresponding atomic types, with the signed and unsigned variants grouped
 *  together. The properties also apply to the corresponding (partial)
 *  specializations of the atomic template. A value of 0 indicates that the
 *  types are never lock-free. A value of 1 indicates that the types are
 *  sometimes lock-free. A value of 2 indicates that the types are always
 *  lock-free.
 *
 * We have a problem when the value is 1: we'd need to check at runtime, but
 * AtomicInteger requires a constexpr answer (defect introduced in Qt 5.0). So
 * we'll err in the side of caution and say it isn't.
 */
template <int N>
struct AtomicTraits
{
   static PHP_DECL_CONSTEXPR inline bool isLockFree();
};

#define PHP_ATOMIC_INT32_IS_SUPPORTED

template<>
struct AtomicOperationsSupport<4>
{
   const int IsSupported = 1;
};

#if ATOMIC_INT_LOCK_FREE == 2
#  define PHP_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE;
#  define PHP_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE;
#  define PHP_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE;
#  define PHP_ATOMIC_INT32_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT32_TEST_AND_SET_IS_ALWAYS_NATIVE;
#  define PHP_ATOMIC_INT32_FETCH_AND_STORE_IS_ALWAYS_NATIVE;
#  define PHP_ATOMIC_INT32_FETCH_AND_ADD_IS_ALWAYS_NATIVE;

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<4>::isLockFree()
{
   return true;
}
   
#elif ATOMIC_INT_LOCK_FREE == 1
#  define PHP_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT32_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT32_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT32_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT32_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

template <> PHP_DECL_CONSTEXPR inline bool AtomicTraits<4>::isLockFree()
{ 
   return false;
}

#else
#  define PHP_ATOMIC_INT_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT_TEST_AND_SET_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT_FETCH_AND_ADD_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT32_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT32_TEST_AND_SET_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT32_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT32_FETCH_AND_ADD_IS_NEVER_NATIVE

template <> PHP_DECL_CONSTEXPR inline bool AtomicTraits<4>::isLockFree()
{ 
   return false; 
}
#endif

#if ATOMIC_POINTER_LOCK_FREE == 2
#  define PHP_ATOMIC_POINTER_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE
#elif ATOMIC_POINTER_LOCK_FREE == 1
#  define PHP_ATOMIC_POINTER_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_POINTER_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_POINTER_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
#else
#  define PHP_ATOMIC_POINTER_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define PHP_ATOMIC_POINTER_TEST_AND_SET_IS_NEVER_NATIVE
#  define PHP_ATOMIC_POINTER_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define PHP_ATOMIC_POINTER_FETCH_AND_ADD_IS_NEVER_NATIVE
#endif


#define PHP_ATOMIC_INT8_IS_SUPPORTED

template<>
struct AtomicOperationsSupport<1>
{
   const int IsSupported = 1;
};

#if ATOMIC_CHAR_LOCK_FREE == 2
#  define PHP_ATOMIC_INT8_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT8_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT8_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT8_FETCH_AND_ADD_IS_ALWAYS_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<1>::isLockFree()
{
   return true;
}

#elif ATOMIC_CHAR_LOCK_FREE == 1
#  define PHP_ATOMIC_INT8_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT8_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT8_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT8_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<1>::isLockFree()
{
   return false;
}

#else

#  define PHP_ATOMIC_INT8_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT8_TEST_AND_SET_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT8_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT8_FETCH_AND_ADD_IS_NEVER_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<1>::isLockFree()
{
   return false;
}
#endif

#define PHP_ATOMIC_INT16_IS_SUPPORTED

template<>
struct AtomicOperationsSupport<2>
{
   const int IsSupported = 1;
};

#if ATOMIC_SHORT_LOCK_FREE == 2

#  define PHP_ATOMIC_INT16_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT16_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT16_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT16_FETCH_AND_ADD_IS_ALWAYS_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<2>::isLockFree()
{
   return true;
}

#elif ATOMIC_SHORT_LOCK_FREE == 1

#  define PHP_ATOMIC_INT16_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT16_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT16_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT16_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<2>::isLockFree()
{
   return false;
}

#else

#  define PHP_ATOMIC_INT16_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT16_TEST_AND_SET_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT16_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT16_FETCH_AND_ADD_IS_NEVER_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<2>::isLockFree()
{
   return false;
}

#endif

#define PHP_ATOMIC_INT64_IS_SUPPORTED

template<>
struct AtomicOperationsSupport<8>
{
   const int IsSupported = 1;
};

#if ATOMIC_LLONG_LOCK_FREE == 2

#  define PHP_ATOMIC_INT64_REFERENCE_COUNTING_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT64_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT64_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define PHP_ATOMIC_INT64_FETCH_AND_ADD_IS_ALWAYS_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<8>::isLockFree()
{
   return true;
}

#else ATOMIC_LLONG_LOCK_FREE == 1

#  define PHP_ATOMIC_INT64_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT64_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT64_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define PHP_ATOMIC_INT64_FETCH_AND_ADD_IS_SOMETIMES_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<8>::isLockFree()
{
   return false;
}

#else

#  define PHP_ATOMIC_INT64_REFERENCE_COUNTING_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT64_TEST_AND_SET_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT64_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define PHP_ATOMIC_INT64_FETCH_AND_ADD_IS_NEVER_NATIVE

template <>
PHP_DECL_CONSTEXPR inline bool AtomicTraits<8>::isLockFree()
{
   return false;
}

#endif


} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_ATOMIC_CXX11_H
