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

#ifndef PHP_KERNEL_THREAD_BASIC_ATOMIC_H
#define PHP_KERNEL_THREAD_BASIC_ATOMIC_H

#include <cstddef>
#include "php/kernel/base/global.h"

#if defined(PHP_COMPILER_ATOMICS) && defined(PHP_COMPILER_CONSTEXPR)
#  include "php/kernel/thread/atomic_cxx11.h"
#else
#  error "php requires c++11 support"
#endif

#if defined(PHP_COMPILER_CONSTEXPR) && defined(PHP_COMPILER_DEFAULT_MEMBERS) && defined(PHP_COMPILER_DELETE_MEMBERS)
#  if defined(PHP_CC_CLANG) && PHP_CC_CLANG < 303
/*
      Do not define PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS for Clang before version 3.3.
      For details about the bug: see http://llvm.org/bugs/show_bug.cgi?id=12670
  */
#  else 
#     define PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
#  endif
#endif

namespace Php {
namespace Kernel {
namespace Thread {

template <typename T>
class BasicAtomicInterger
{
public:
   using OperationsType = AtomicOperations<T>;
   //这个地方将来使用Concept优化
//   PHP_STATIC_ASSERT_X(TypeInfo<T>::isIntegral, "template parameter is not an integral type");
//   PHP_STATIC_ASSERT_X(AtomicOpsSupport<sizeof(T)>::IsSupported, "template parameter is an integral of a size not supported on this platform");
   typename OperationsType::Type m_atomicValue;
   // Everything below is either implemented in qatomic_XXX.h or (as fallback) in generic_atomic.h
   
   T load() const PHP_DECL_NOTHROW
   {
      return OperationsType::load(m_atomicValue);
   }
   
   T loadAcquire() const PHP_DECL_NOEXCEPT
   {
      return OperationsType::loadAcquire(m_atomicValue);
   }
   
   void store(T newValue) PHP_DECL_NOTHROW
   {
      OperationsType::load(m_atomicValue, newValue);
   }
   
   void storeRelease(T newValue) PHP_DECL_NOTHROW
   {
      OperationsType::storeRelease(m_atomicValue, newValue);
   }
   
   operator T() const PHP_DECL_NOEXCEPT
   {
      return loadAcquire();
   }
   
   T operator=(T newValue) PHP_DECL_NOEXCEPT
   {
      OperationsType::storeRelease(newValue);
      return newValue;
   }
   
   static PHP_DECL_CONSTEXPR bool isReferenceCountingNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isReferenceCountingNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isReferenceCountingWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isReferenceCountingWaitFree();
   }
   
   bool ref() PHP_DECL_NOEXCEPT
   {
      return OperationsType::ref(m_atomicValue);
   }
   
   bool deref() PHP_DECL_NOEXCEPT
   {
      return OperationsType::deref(m_atomicValue);
   }
   
   static PHP_DECL_CONSTEXPR bool isTestAndSetNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isTestAndSetNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isTestAndSetWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isTestAndSetWaitFree();
   }
   
   bool testAndSetRelaxed(T expectValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelaxed(m_atomicValue, expectValue, newValue);
   }
   
   bool testAndSetAcquire(T expectValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetAcquire(m_atomicValue, expectValue, newValue);
   }
   
   bool testAndSetRelease(T expectValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelease(m_atomicValue, expectValue, newValue);
   }
   
   bool testAndSetOrdered(T expectValue, T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetOrdered(m_atomicValue, expectValue, newValue);
   }
   
   bool testAndSetRelaxed(T expectValue, T newValue, T &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelaxed(m_atomicValue, expectValue, newValue, &currentValue);
   }
   
   bool testAndSetAcquire(T expectValue, T newValue, T &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetAcquire(m_atomicValue, expectValue, newValue, &currentValue);
   }
   
   bool testAndSetRelease(T expectValue, T newValue, T &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelease(m_atomicValue, expectValue, newValue, &currentValue);
   }
   
   bool testAndSetOrdered(T expectValue, T newValue, T &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetOrdered(m_atomicValue, expectValue, newValue, &currentValue);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndStoreNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndStoreNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndStoreWaitFree();
   }
   
   T fetchAndStoreRelaxed(T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreRelaxed(m_atomicValue, newValue);
   }
   
   T fetchAndStoreAcquire(T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreAcquire(m_atomicValue, newValue);
   }
   
   T fetchAndStoreRelease(T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreRelease(m_atomicValue, newValue);
   }
   
   T fetchAndStoreOrdered(T newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreOrdered(m_atomicValue, newValue);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAddNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndAddNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAddWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndAddWaitFree();
   }
   
   T fetchAndAddRelaxed(T valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddRelaxed(m_atomicValue, valueToAdd);
   }
   
   T fetchAndAddAcquire(T valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddAcquire(m_atomicValue, valueToAdd);
   }
   
   T fetchAndAddRelease(T valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddRelease(m_atomicValue, valueToAdd);
   }
   
   T fetchAndAddOrdered(T valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddOrdered(m_atomicValue, valueToAdd);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndSubNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndSubNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndSubWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndSubWaitFree();
   }
   
   T fetchAndSubRelaxed(T valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubRelaxed(m_atomicValue, valueToSub);
   }
   
   T fetchAndSubAcquire(T valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubAcquire(m_atomicValue, valueToSub);
   }
   
   T fetchAndSubRelease(T valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubRelease(m_atomicValue, valueToSub);
   }
   
   T fetchAndSubOrdered(T valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubOrdered(m_atomicValue, valueToSub);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAndNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndAndNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAndWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndAndWaitFree();
   }
   
   T fetchAndAndRelaxed(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAndRelaxed(m_atomicValue, operand);
   }
   
   T fetchAndAndAcquire(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAndAcquire(m_atomicValue, operand);
   }
   
   T fetchAndAndRelease(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAndRelease(m_atomicValue, operand);
   }
   
   T fetchAndAndOrdered(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAndOrdered(m_atomicValue, operand);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndOrNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndOrNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndOrWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndOrWaitFree();
   }
   
   T fetchAndOrRelaxed(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndOrRelaxed(m_atomicValue, operand);
   }
   
   T fetchAndOrAcquire(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndOrAcquire(m_atomicValue, operand);
   }
   
   T fetchAndOrRelease(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndOrRelease(m_atomicValue, operand);
   }
   
   T fetchAndOrOrdered(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndOrOrdered(m_atomicValue, operand);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndXorNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndXorNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndXorWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndXorWaitFree();
   }
   
   T fetchAndXorRelaxed(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndXorRelaxed(m_atomicValue, operand);
   }
   
   T fetchAndXorAcquire(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndXorAcquire(m_atomicValue, operand);
   }
   
   T fetchAndXorRelease(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndXorRelease(m_atomicValue, operand);
   }
   
   T fetchAndXorOrdered(T operand) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndXorOrdered(m_atomicValue, operand);
   }
   
   T operator++() PHP_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1) + 1;
   }
   
   T operator++(int) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddOrdered(1);
   }
   
   T operator--() PHP_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1) - 1;
   }
   
   T operator--(int) PHP_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1);  
   }
   
   T operator+=(T v) PHP_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(v) + v;
   }
   
   T operator-=(T v) PHP_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(v) - v;
   }
   
   T operator &=(T v) PHP_DECL_NOEXCEPT
   {
      return fetchAndAndOrdered(v) & v;
   }
   
   T operator |=(T v) PHP_DECL_NOEXCEPT
   {
      return fetchAndOrOrdered(v) | v;
   }
   
   T operator ^=(T v) PHP_DECL_NOEXCEPT
   {
      return fetchAndXorOrdered(v) ^ v;
   }
   
#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
   BasicAtomicInterger() = default;
   PHP_DECL_CONSTEXPR BasicAtomicInterger(T value) PHP_DECL_NOEXCEPT : m_atomicValue(value)
   {}
   BasicAtomicInterger(const BasicAtomicInterger &) = delete;
   BasicAtomicInterger &operator=(const BasicAtomicInterger &) = delete;
   BasicAtomicInterger &operator=(const BasicAtomicInterger &) volatile = delete;
#endif
   
};

using BasicAtomicInt = BasicAtomicInterger<int>;

template <typename X>
class BasicAtomicPointer
{
public:
   using Type = X *;
   using OperationsType = AtomicOperations<Type>;
   using AtomicType = typename OperationsType::Type;
public:
   AtomicType m_atomicValue;
public:
   Type load() const PHP_DECL_NOEXCEPT
   {
      return OperationsType::load(m_atomicValue);
   }
   
   Type loadAcquire() const PHP_DECL_NOEXCEPT
   {
      return OperationsType::loadAcquire(m_atomicValue);
   }
   
   void store(Type newValue) PHP_DECL_NOEXCEPT
   {
      OperationsType::store(m_atomicValue, newValue);
   }
   
   void storeRelease(Type newValue) PHP_DECL_NOEXCEPT
   {
      OperationsType::storeRelease(m_atomicValue, newValue);
   }
   
   operator Type() const PHP_DECL_NOEXCEPT
   {
      return loadAcquire();
   }
   
   Type operator =(Type newValue) PHP_DECL_NOEXCEPT
   {
      storeRelease(newValue);
      return newValue;
   }
   
   static PHP_DECL_CONSTEXPR bool isTestAndSetNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isTestAndSetNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isTestAndSetWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isTestAndSetWaitFree();
   }
   
   bool testAndSetRelaxed(Type expectedValue, Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelaxed(m_atomicValue, expectedValue, newValue);
   }
   
   bool testAndSetAcquire(Type expectedValue, Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetAcquire(m_atomicValue, expectedValue, newValue);
   }
   
   bool testAndSetRelease(Type expectedValue, Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelease(m_atomicValue, expectedValue, newValue);
   }
   
   bool testAndSetOrdered(Type expectedValue, Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetOrdered(m_atomicValue, expectedValue, newValue);
   }
   
   
   bool testAndSetRelaxed(Type expectedValue, Type newValue, Type &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelaxed(m_atomicValue, expectedValue, newValue, &currentValue);
   }
   
   bool testAndSetAcquire(Type expectedValue, Type newValue, Type &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetAcquire(m_atomicValue, expectedValue, newValue, &currentValue);
   }
   
   bool testAndSetRelease(Type expectedValue, Type newValue, Type &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetRelease(m_atomicValue, expectedValue, newValue), &currentValue;
   }
   
   bool testAndSetOrdered(Type expectedValue, Type newValue, Type &currentValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::testAndSetOrdered(m_atomicValue, expectedValue, newValue, &currentValue);
   }
   
   
   static PHP_DECL_CONSTEXPR bool isFetchAndStoreNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndStoreNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndStoreWaitFree();
   }
   
   Type fetchAndStoreRelaxed(Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreRelaxed(m_atomicValue, newValue);
   }
   
   Type fetchAndStoreAcquire(Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreAcquire(m_atomicValue, newValue);
   }
   
   Type fetchAndStoreRelease(Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreRelease(m_atomicValue, newValue);
   }
   
   Type fetchAndStoreOrdered(Type newValue) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndStoreOrdered(m_atomicValue, newValue);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAddNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndAddNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAddWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndAddWaitFree();
   }
   
   Type fetchAndAddRelaxed(Type valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddRelaxed(m_atomicValue, valueToAdd);
   }
   
   Type fetchAndAddAcquire(Type valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddAcquire(m_atomicValue, valueToAdd);
   }
   
   Type fetchAndAddRelease(Type valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddRelease(m_atomicValue, valueToAdd);
   }
   
   Type fetchAndAddOrdered(Type valueToAdd) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndAddOrdered(m_atomicValue, valueToAdd);
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndSubNative() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndSubNative();
   }
   
   static PHP_DECL_CONSTEXPR bool isFetchAndSubWaitFree() PHP_DECL_NOEXCEPT
   {
      return OperationsType::isFetchAndSubWaitFree();
   }
   
   Type fetchAndSubRelaxed(Type valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubRelaxed(m_atomicValue, valueToSub);
   }
   
   Type fetchAndSubAcquire(Type valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubAcquire(m_atomicValue, valueToSub);
   }
   
   Type fetchAndSubRelease(Type valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubRelease(m_atomicValue, valueToSub);
   }
   
   Type fetchAndSubOrdered(Type valueToSub) PHP_DECL_NOEXCEPT
   {
      return OperationsType::fetchAndSubOrdered(m_atomicValue, valueToSub);
   }
   
   Type operator ++() PHP_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1) + 1;
   }
   
   Type operator ++(int) PHP_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1);
   }
   
   Type operator --() PHP_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1) - 1;
   }
   
   Type operator --(int) PHP_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1);
   }
   
   Type operator +=(std::ptrdiff_t valueToAdd) PHP_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(valueToAdd) + valueToAdd;
   }
   
   Type operator -=(std::ptrdiff_t valueToSub) PHP_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(valueToSub) - valueToSub;
   }

#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
   BasicAtomicPointer() = default;
   BasicAtomicPointer(const BasicAtomicPointer &) = delete;
   PHP_DECL_CONSTEXPR BasicAtomicPointer(Type value) PHP_DECL_NOEXCEPT : m_atomicValue(value){}
   BasicAtomicPointer &operator =(const BasicAtomicPointer &) = delete;
   BasicAtomicPointer &operator =(const BasicAtomicPointer &) volatile = delete;
#endif
};

#ifndef PHP_BASIC_ATOMIC_INITIALIZER
#  define PHP_BASIC_ATOMIC_INITIALIZER(a) { (a) } 
#endif

} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_BASIC_ATOMIC_H
