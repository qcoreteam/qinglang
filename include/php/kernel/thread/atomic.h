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
#ifndef PHP_KERNEL_THREAD_ATOMIC_H
#define PHP_KERNEL_THREAD_ATOMIC_H

#include "php/kernel/thread/basic_atomic.h"

namespace Php {
namespace Kernel {
namespace Thread {

template <typename T>
class AtomicInteger : public BasicAtomicInterger<T>
{
public:
#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
   PHP_DECL_CONSTEXPR AtomicInteger(T value = 0) PHP_DECL_NOEXCEPT : BasicAtomicInterger<T>(value)
   {}
#else
   inline AtomicInteger(T value = 0) PHP_DECL_NOEXCEPT
   {
      m_atomicValue = value;
   }
#endif
   
   inline AtomicInteger(const AtomicInteger& other) PHP_DECL_NOEXCEPT
#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
      : BasicAtomicInterger<T>(other)
   #endif
   {
      storeRelease(other.loadAcquire());
   }
   
   inline AtomicInteger &operator =(const AtomicInteger &other) PHP_DECL_NOEXCEPT
   {
      storeRelease(other.loadAcquire());
      return *this;
   }
   
   T load() const PHP_DECL_NOEXCEPT;
   T loadAcquire() const PHP_DECL_NOEXCEPT;
   void store(T newValue) PHP_DECL_NOEXCEPT;
   void storeRelease(T newValue) PHP_DECL_NOEXCEPT;
   operator T() const;
   AtomicInteger &operator=(T newValue);
   
   static PHP_DECL_CONSTEXPR bool isReferenceCountingNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isReferenceCountingWaitFree() PHP_DECL_NOEXCEPT;
   
   bool ref() PHP_DECL_NOEXCEPT;
   bool deref() PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isTestAndSetNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isTestAndSetWaitFree() PHP_DECL_NOEXCEPT;
   
   bool testAndSetRelaxed(T expectedValue, T newValue) PHP_DECL_NOEXCEPT;
   bool testAndSetAcquire(T expectedValue, T newValue) PHP_DECL_NOEXCEPT;
   bool testAndSetRelease(T expectedValue, T newValue) PHP_DECL_NOEXCEPT;
   bool testAndSetOrdered(T expectedValue, T newValue) PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isFetchAndStoreNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() PHP_DECL_NOEXCEPT;
   
   T fetchAndStoreRelaxed(T newValue) PHP_DECL_NOEXCEPT;
   T fetchAndStoreAcquire(T newValue) PHP_DECL_NOEXCEPT;
   T fetchAndStoreRelease(T newValue) PHP_DECL_NOEXCEPT;
   T fetchAndStoreOrdered(T newValue) PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAddNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isFetchAndAddWaitFree() PHP_DECL_NOEXCEPT;
   
   T fetchAndAddRelaxed(T valueToAdd) PHP_DECL_NOEXCEPT;
   T fetchAndAddAcquire(T valueToAdd) PHP_DECL_NOEXCEPT;
   T fetchAndAddRelease(T valueToAdd) PHP_DECL_NOEXCEPT;
   T fetchAndAddOrdered(T valueToAdd) PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isFetchAndSubNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isFetchAndSubWaitFree() PHP_DECL_NOEXCEPT;
   
   T fetchAndSubRelaxed(T valueToSub) PHP_DECL_NOEXCEPT;
   T fetchAndSubAcquire(T valueToSub) PHP_DECL_NOEXCEPT;
   T fetchAndSubRelease(T valueToSub) PHP_DECL_NOEXCEPT;
   T fetchAndSubOrdered(T valueToSub) PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isFetchAndAndNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isFetchAndAndWaitFree() PHP_DECL_NOEXCEPT;
   
   T fetchAndAndRelaxed(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndAndAcquire(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndAndRelease(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndAndOrdered(T operand) PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isFetchAndOrNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isFetchAndOrWaitFree() PHP_DECL_NOEXCEPT;
   
   T fetchAndOrRelaxed(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndOrAcquire(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndOrRelease(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndOrOrdered(T operand) PHP_DECL_NOEXCEPT;
   
   static PHP_DECL_CONSTEXPR bool isFetchAndXorNative() PHP_DECL_NOEXCEPT;
   static PHP_DECL_CONSTEXPR bool isFetchAndXorWaitFree() PHP_DECL_NOEXCEPT;
   
   T fetchAndXorRelaxed(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndXorAcquire(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndXorRelease(T operand) PHP_DECL_NOEXCEPT;
   T fetchAndXorOrdered(T operand) PHP_DECL_NOEXCEPT;
   
   T operator++() PHP_DECL_NOEXCEPT;
   T operator++(int) PHP_DECL_NOEXCEPT;
   T operator--() PHP_DECL_NOEXCEPT;
   T operator--(int) PHP_DECL_NOEXCEPT;
   T operator+=(T value) PHP_DECL_NOEXCEPT;
   T operator-=(T value) PHP_DECL_NOEXCEPT;
   T operator|=(T value) PHP_DECL_NOEXCEPT;
   T operator&=(T value) PHP_DECL_NOEXCEPT;
   T operator^=(T value) PHP_DECL_NOEXCEPT;
   
};

class AtomicInt : public AtomicInteger<int>
{
public:
#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
   constexpr
#endif
   AtomicInt(int value = 0) PHP_DECL_NOEXCEPT : AtomicInteger<int>(value) {}
};

template<typename T>
class AtomicPointer : public BasicAtomicInterger<T>
{
public:
#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
   PHP_DECL_CONSTEXPR AtomicPointer(T *value = nullptr) PHP_DECL_NOEXCEPT : BasicAtomicPointer<T>(value){}
#else
   inline AtomicPointer(T *value = 0) PHP_DECL_NOEXCEPT
   {
      store(value);
   }
#endif
   inline AtomicPointer(const AtomicPointer<T> &other) PHP_DECL_NOEXCEPT
   {
      storeRelease(other.loadAcquire());
   }
   
   inline AtomicPointer &operator =(const  AtomicPointer<T> &other) PHP_DECL_NOEXCEPT
   {
      storeRelease(other.loadAcquire());
      return *this;
   }
   
   T *load() const PHP_DECL_NOEXCEPT;
       T *loadAcquire() const PHP_DECL_NOEXCEPT;
       void store(T *newValue) PHP_DECL_NOEXCEPT;
       void storeRelease(T *newValue) PHP_DECL_NOEXCEPT;
   
       static PHP_DECL_CONSTEXPR bool isTestAndSetNative() PHP_DECL_NOEXCEPT;
       static PHP_DECL_CONSTEXPR bool isTestAndSetWaitFree() PHP_DECL_NOEXCEPT;
   
       bool testAndSetRelaxed(T *expectedValue, T *newValue) PHP_DECL_NOEXCEPT;
       bool testAndSetAcquire(T *expectedValue, T *newValue) PHP_DECL_NOEXCEPT;
       bool testAndSetRelease(T *expectedValue, T *newValue) PHP_DECL_NOEXCEPT;
       bool testAndSetOrdered(T *expectedValue, T *newValue) PHP_DECL_NOEXCEPT;
   
       static PHP_DECL_CONSTEXPR bool isFetchAndStoreNative() PHP_DECL_NOEXCEPT;
       static PHP_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() PHP_DECL_NOEXCEPT;
   
       T *fetchAndStoreRelaxed(T *newValue) PHP_DECL_NOEXCEPT;
       T *fetchAndStoreAcquire(T *newValue) PHP_DECL_NOEXCEPT;
       T *fetchAndStoreRelease(T *newValue) PHP_DECL_NOEXCEPT;
       T *fetchAndStoreOrdered(T *newValue) PHP_DECL_NOEXCEPT;
   
       static PHP_DECL_CONSTEXPR bool isFetchAndAddNative() PHP_DECL_NOEXCEPT;
       static PHP_DECL_CONSTEXPR bool isFetchAndAddWaitFree() PHP_DECL_NOEXCEPT;
   
       T *fetchAndAddRelaxed(std::ptrdiff_t valueToAdd) PHP_DECL_NOEXCEPT;
       T *fetchAndAddAcquire(std::ptrdiff_t valueToAdd) PHP_DECL_NOEXCEPT;
       T *fetchAndAddRelease(std::ptrdiff_t valueToAdd) PHP_DECL_NOEXCEPT;
       T *fetchAndAddOrdered(std::ptrdiff_t valueToAdd) PHP_DECL_NOEXCEPT;
       
       static PHP_DECL_CONSTEXPR bool isFetchAndSubNative() PHP_DECL_NOEXCEPT;
       static PHP_DECL_CONSTEXPR bool isFetchAndSubWaitFree() PHP_DECL_NOEXCEPT;
   
       T *fetchAndSubRelaxed(std::ptrdiff_t valueToSub) PHP_DECL_NOEXCEPT;
       T *fetchAndSubAcquire(std::ptrdiff_t valueToSub) PHP_DECL_NOEXCEPT;
       T *fetchAndSubRelease(std::ptrdiff_t valueToSub) PHP_DECL_NOEXCEPT;
       T *fetchAndSubOrdered(std::ptrdiff_t valueToSub) PHP_DECL_NOEXCEPT;
};

#ifdef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
#  undef PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
#endif

/*
 * @TODO 使用 Concept 优化
 */
template <typename T>
inline void atomic_assign(T *&d, T *x)
{
   if(d == x){
      return;
   }
   x->ref().ref();
   if(!d->ref.deref()){
      delete d;
   }
}

template<typename T>
inline void atomic_detach(T *&d)
{
   if(d->ref().load() == 1){
      return;
   }
   T *x = d;
   d = new T(*d);
   if(!x->ref().deref()){
      delete x;
   }
}

} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_ATOMIC_H
