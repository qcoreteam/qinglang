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
#ifndef PHP_KERNEL_BASE_TYPE_TRAITS_H
#define PHP_KERNEL_BASE_TYPE_TRAITS_H

#include "php/kernel/base/global.h"
#include <type_traits>

PHP_BEGIN_G_NAMESPACE

/*
 * 检查相关的类是否具有相关的方法
 */
#define PHP_METHOD_CHECKER(className, method, retType, args) \
template<typename Class, typename = void> \
struct className : std::false_type{}; \
template<typename Class> \
struct className<Class, typename std::enable_if< \
      std::is_convertible<decltype(std::declval<Class>().method args), retType>::value>::type> \
: std::true_type{}

PHP_END_NAMESPACE

#endif // PHP_KERNEL_BASE_TYPE_TRAITS_H
