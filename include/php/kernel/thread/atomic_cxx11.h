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

#elif ATOMIC_LLONG_LOCK_FREE == 1

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

template <typename X>
struct AtomicOperations
{
   using Type = std::atomic<X>;
   template <typename T>
   static inline T load(const std::atomic<T> &atomicValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_relaxed);
   };
   
   template <typename T>
   static inline T load(const volatile std::atomic<T> &atomicValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T loadAcquire(const std::atomic<T> &atomicValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T loadAcquire(const volatile std::atomic<T> &atomicValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T store(const std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.store(newValue, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T store(const volatile std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.store(newValue, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T storeRelease(const std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.store(newValue, std::memory_order_release);
   }
   
   template <typename T>
   static inline T storeRelease(const volatile std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.store(newValue, std::memory_order_release);
   }
   
   static inline PHP_DECL_CONSTEXPR bool isReferenceCountingNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_DECL_CONSTEXPR bool isReferenceCountingWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static inline bool ref(std::atomic<T> &atomicValue) PHP_DECL_NOEXCEPT
   {
      return ++atomicValue != 0;
   }
   
   template <typename T>
   static inline bool deref(std::atomic<T> &atomicValue) PHP_DECL_NOEXCEPT
   {
      return --atomicValue != 0;
   }
   // TestAndSet 相关接口
   static inline PHP_DECL_CONSTEXPR bool isTestAndSetNative() PHP_DECL_NOEXCEPT
   {
      return AtomicTraits<sizeof(X)>::isLockFree();
   }
   
   static inline PHP_DECL_CONSTEXPR bool isTestAndSetWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static bool testAndSetRelaxed(std::atomic<T> &atomicValue, T expectValue, T newValue, T *currentValue = nullptr) PHP_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectValue, newValue, std::memory_order_relaxed);
      if(nullptr != currentValue){
         *currentValue = expectValue;
      }
      return tmp;
   }
   
   template <typename T>
   static bool testAndSetAcquire(std::atomic<T> &atomicValue, T expectValue, T newValue, T *currentValue = nullptr) PHP_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectValue, newValue, std::memory_order_acquire);
      if(nullptr != currentValue){
         *currentValue = expectValue;
      }
      return tmp;
   }
   
   template <typename T>
   static bool testAndSetRelease(std::atomic<T> &atomicValue, T expectValue, T newValue, T *currentValue = nullptr) PHP_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectValue, newValue, std::memory_order_release);
      if(nullptr != currentValue){
         *currentValue = expectValue;
      }
      return tmp;
   }
   
   template <typename T>
   static bool testAndSetOrdered(std::atomic<T> &atomicValue, T expectValue, T newValue, T *currentValue = nullptr) PHP_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectValue, newValue, std::memory_order_acq_rel);
      if(nullptr != currentValue){
         *currentValue = expectValue;
      }
      return tmp;
   }
   
   // FetchAndStore 接口
   static inline PHP_DECL_CONSTEXPR bool isFetchAndStoreNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static T fetchAndStoreRelaxed(std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_relaxed);
   }
   
   template <typename T>
   static T fetchAndStoreAcquire(std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_acquire);
   }
   
   template <typename T>
   static T fetchAndStoreRelease(std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_release);
   }
   
   template <typename T>
   static T fetchAndStoreOrdered(std::atomic<T> &atomicValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_acq_rel);
   }
   
   // FetchAndAdd
   
   static inline PHP_CONSTEXPR bool isFetchAndAddNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_CONSTEXPR bool isFetchAndAddWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static inline T fetchAndAddRelaxed(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(valueToAdd, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T fetchAndAddAcquire(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(valueToAdd, std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T fetchAndAddRelease(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(valueToAdd, std::memory_order_release);
   }
   
   template <typename T>
   static inline T fetchAndAddOrdered(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(valueToAdd, std::memory_order_acq_rel);
   }
   
   // FetchAndSub
   
   static inline PHP_CONSTEXPR bool isFetchAndSubNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_CONSTEXPR bool isFetchAndSubWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static inline T fetchAndSubRelaxed(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(valueToSub, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T fetchAndSubAcquire(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(valueToSub, std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T fetchAndSubRelease(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(valueToSub, std::memory_order_release);
   }
   
   template <typename T>
   static inline T fetchAndSubOrdered(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(valueToSub, std::memory_order_acq_rel);
   }
   
   // FetchAndAnd
   
   static inline PHP_CONSTEXPR bool isFetchAndAndNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_CONSTEXPR bool isFetchAndAndWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static inline T fetchAndAndRelaxed(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAnd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(valueToAnd, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T fetchAndAndAcquire(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAnd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(valueToAnd, std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T fetchAndAndRelease(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAnd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(valueToAnd, std::memory_order_release);
   }
   
   template <typename T>
   static inline T fetchAndAndOrdered(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAnd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(valueToAnd, std::memory_order_acq_rel);
   }
   
   // FetchAndOr
   
   static inline PHP_CONSTEXPR bool isFetchAndOrNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_CONSTEXPR bool isFetchAndOrWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static inline T fetchAndOrRelaxed(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToOr) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(valueToOr, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T fetchAndOrAcquire(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToOr) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(valueToOr, std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T fetchAndOrRelease(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToOr) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(valueToOr, std::memory_order_release);
   }
   
   template <typename T>
   static inline T fetchAndOrdered(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToAnd) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(valueToOr, std::memory_order_acq_rel);
   }
   
   // FetchAndXor
   
   static inline PHP_CONSTEXPR bool isFetchAndXorNative() PHP_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }
   
   static inline PHP_CONSTEXPR bool isFetchAndXorWaitFree() PHP_DECL_NOEXCEPT
   {
      return false;
   }
   
   template <typename T>
   static inline T fetchAndXorRelaxed(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToXor) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(valueToXor, std::memory_order_relaxed);
   }
   
   template <typename T>
   static inline T fetchAndXorAcquire(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToXor) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(valueToXor, std::memory_order_acquire);
   }
   
   template <typename T>
   static inline T fetchAndXorRelease(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToXor) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(valueToXor, std::memory_order_release);
   }
   
   template <typename T>
   static inline T fetchAndXordered(std::atomic<T> &atomicValue, typename AtomicAdditiveType<T>::AdditiveType valueToXor) PHP_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(valueToXor, std::memory_order_acq_rel);
   }
};

#if defined(PHP_COMPILER_CONSTEXPR) && defined(PHP_COMPILER_DEFAULT_MEMBERS) && defined(PHP_COMPILER_DELETE_MEMBERS)
#  define PHP_BASIC_ATOMIC_INITIALIZER(a) { a }
#else
#  define PHP_BASIC_ATOMIC_INITIALIZER(a) { ATOMIC_VAR_INIT(a) }
#endif

} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_ATOMIC_CXX11_H
