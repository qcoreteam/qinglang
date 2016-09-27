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

#endif // PHP_KERNEL_BASE_GLOBAL_H