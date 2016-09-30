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

template <typename T>
using TypeInfo = Php::Kernel::Base::TypeInfo<T>;

template<int> 
struct AtomicOperationsSupport
{
   const int IsSupported = 0;
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
   
   template <typename T>
   static void orderedMemoryFence(const T &) PHP_DECL_NOTHROW
   {
      
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T load(const T &value) PHP_DECL_NOTHROW
   {
      return value;
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE void store(T &value, X newValue) PHP_DECL_NOTHROW
   {
      value = newValue;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T loadAcquire(const T &value) PHP_DECL_NOTHROW
   {
      T tmp = *static_cast<typename std::add_cv<T *>::type>(&value);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE void storeRelease(T &value, X newValue) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      *static_cast<typename std::add_volatile<T *>::type>(&value) = value;
   }
   
   static inline PHP_DECL_CONSTEXPR bool isReferenceCountingNative() PHP_DECL_NOTHROW
   {
      return BaseClass::isFetchAndAddNative();
   }
   
   static inline PHP_DECL_CONSTEXPR bool isReferenceCountingWaitFree() PHP_DECL_NOTHROW
   {
      return BaseClass::isFetchAndAddWaitFree();
   }
   
   template <typename T> 
   static PHP_ALWAYS_INLINE bool ref(T & value) PHP_DECL_NOTHROW
   {
      return BaseClass::fetchAndAddRelaxed(value, 1) != T(-1);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE bool deref(T & value) PHP_DECL_NOTHROW
   {
      return BaseClass::fetchAndAddRelaxed(value, -1) != 1;
   }
   
#if 0
   // These functions have no default implementation
   // 在这里的函数只是起提示开发者的作用
   static inline PHP_DECL_CONSTEXPR bool isTestAndSetNative() PHP_DECL_NOTHROW;
   static inline PHP_DECL_CONSTEXPR bool isTestAndSetWaitFree() PHP_DECL_NOTHROW;
   template <typename T, typename X>
   static inline bool testAndSetRelaxed(T &value, X expectedValue, X newValue) PHP_DECL_NOTHROW;
   template <typename T, typename X>
   static inline bool testAndSetRelaxed(T &value, X expectedValue, X newValue, X *currentValue) PHP_DECL_NOTHROW;
#endif
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE bool testAndSetAcquire(T &value, X expectedValue, X newValue) PHP_DECL_NOTHROW
   {
      bool tmp = BaseClass::testAndSetRelaxed(value, expectedValue, newValue);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE bool testAndSetRelease(T &value, X expectedValue, X newValue) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::testAndSetRelaxed(value, expectedValue, newValue);
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE bool testAndSetOrdered(T &value, X expectedValue, X newValue) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::testAndSetRelaxed(value, expectedValue, newValue);
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE bool testAndSetAcquire(T &value, X expectedValue, X newValue, X *currentValue) PHP_DECL_NOTHROW
   {
      bool tmp = BaseClass::testAndSetRelaxed(value, expectedValue, newValue, currentValue);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE bool testAndSetRelease(T &value, X expectedValue, X newValue, X *currentValue) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::testAndSetRelaxed(value, expectedValue, newValue, currentValue);
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE bool testAndSetOrdered(T &value, X expectedValue, X newValue, X *currentValue) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::testAndSetRelaxed(value, expectedValue, newValue, currentValue);
   }
   
   static inline PHP_DECL_CONSTEXPR bool isFetchAndStoreNative() PHP_DECL_NOTHROW
   {
      return false;
   }
   
   static inline PHP_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() PHP_DECL_NOTHROW
   {
      return false;
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE T fetchAndStoreRelaxed(T &value, X newValue) PHP_DECL_NOTHROW
   {
      // implement fetchAndStore on top of testAndSet
      while(true){
         T tmp = load(value);
         if(BaseClass::testAndSetRelaxed(value, tmp, newValue)){
            return tmp;
         }
      }
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE T fetchAndStoreAcquire(T &value, X newValue) PHP_DECL_NOTHROW
   {
      T tmp = BaseClass::fetchAndStoreRelaxed(value, newValue);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE T fetchAndStoreRelease(T &value, X newValue) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::fetchAndStoreRelaxed(value, newValue);
   }
   
   template <typename T, typename X>
   static PHP_ALWAYS_INLINE T fetchAndStoreOrdered(T &value, X newValue) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::fetchAndStoreRelaxed(value, newValue);
   }
   
   static inline PHP_DECL_CONSTEXPR bool isFetchAndAddNative() PHP_DECL_NOTHROW
   {
      return false;
   }
   
   static inline PHP_DECL_CONSTEXPR bool isFetchAndAddWaitFree() PHP_DECL_NOTHROW
   {
      return false;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAddRelaxed(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_NORETURN
   {
      // implement fetchAndAdd on top of testAndSet
      while(true){
         T tmp = BaseClass::load(value);
         if(BaseClass::testAndSetRelaxed(value, tmp, T(tmp + valueToAdd))){
            return tmp;
         }
      }
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAddAcquire(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOTHROW
   {
      T tmp = BaseClass::fetchAndAddRelaxed(value, valueToAdd);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAddRelease(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::fetchAndAddRelaxed(value, valueToAdd);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAddOrdered(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToAdd) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::fetchAndAddRelaxed(value, valueToAdd);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndSubRelaxed(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOTHROW
   {
      return fetchAndAddRelaxed(value, -valueToSub);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndSubAcquire(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOTHROW
   {
      T tmp = BaseClass::fetchAndSubRelaxed(value, valueToSub);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndSubRelease(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::fetchAndSubRelaxed(value, value);;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndSubOrdered(T &value, typename AtomicAdditiveType<T>::AdditiveType valueToSub) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::fetchAndSubOrdered(value, valueToSub);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAndRelaxed(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      // implement fetchAndAnd on top of testAndSet
      T tmp = BaseClass::load(value);
      while(true){
         if(BaseClass::testAndSetRelaxed(value, tmp, T(tmp & operand), &tmp)){
            return tmp;
         }
      }
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAndAcquire(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      T tmp = BaseClass::fetchAndAndRelaxed(value, operand);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAndRelease(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::fetchAndAndRelaxed(value, operand);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndAndOrdered(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::fetchAndAndRelaxed(value, operand);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndOrRelaxed(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      // implement fetchAndOr on top of testAndSet
      T tmp = BaseClass::load(value);
      while(true){
         if(BaseClass::testAndSetRelaxed(value, tmp, T(tmp | operand), &tmp)){
            return tmp;
         }
      }
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndOrAcquire(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      T tmp = BaseClass::fetchAndOrRelaxed(value, operand);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndOrRelease(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::fetchAndOrRelaxed(value, operand);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndOrOrdered(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::fetchAndOrRelaxed(value, operand);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndXorRelaxed(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      // implement fetchAndXor on top of testAndSet
      T tmp = BaseClass::load(value);
      while(true){
         if(BaseClass::testAndSetRelaxed(value, tmp, T(tmp ^ operand), &tmp)){
            return tmp;
         }
      }
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndXorAcquire(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      T tmp = BaseClass::fetchAndXorAcquire(value, operand);
      BaseClass::acquireMemoryFence(value);
      return tmp;
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndXorRelease(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      BaseClass::releaseMemoryFence(value);
      return BaseClass::fetchAndXorRelaxed(value, operand);
   }
   
   template <typename T>
   static PHP_ALWAYS_INLINE T fetchAndXorOrdered(T &value, typename std::enable_if<TypeInfo<T>::isIntegral, T>::type operand) PHP_DECL_NOTHROW
   {
      BaseClass::orderedMemoryFence(value);
      return BaseClass::fetchAndXorRelaxed(value, operand);
   }
};

} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_GENERIC_ATOMIC_H
