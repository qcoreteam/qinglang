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

#include "php/kernel/base/global.h"

#if defined(PHP_COMPILER_ATOMICS) && defined(PHP_COMPILER_CONSTEXPR)
#  include "php/kernel/thread/atomic_cxx11.h"
#else
#  error "php requires c++11 support"
#endif

#if defined(PHP_COMPILER_CONSTEXPR) && defined(PHP_COMPILER_DEFAULT_MEMBERS) && defined(PHP_COMPILER_DELETE_MEMBERS)
#  if defined(PHP_CC_CLANG) && PHP_CC_CLANG < 303
/*
      Do not define QT_BASIC_ATOMIC_HAS_CONSTRUCTORS for Clang before version 3.3.
      For details about the bug: see http://llvm.org/bugs/show_bug.cgi?id=12670
  */
#  else 
#     define PHP_BASIC_ATOMIC_HAS_CONSTRUCTORS
#  endif
#endif

namespace Php {
namespace Kernel {
namespace Thread {



} // Thread
} // Kernel
} // Php

#endif // PHP_KERNEL_THREAD_BASIC_ATOMIC_H
