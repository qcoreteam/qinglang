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
#ifndef PHP_KERNEL_BASE_SYSTEM_DETECTION_H
#define PHP_KERNEL_BASE_SYSTEM_DETECTION_H

/*
     php++ 支持的操作系统必须是一下的其中一种
     MACOS -- macOS
     LINUX    - Linux [has variants]
     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     FREEBSD  - FreeBSD [has variants]
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     BSDI     - BSD/OS
     HURD     - GNU Hurd
     UNIXWARE - UnixWare 7, Open UNIX 8
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
     以下的操作系统具有变种：
     FREEBSD  - PHP_OS_FREEBSD is defined only when building for FreeBSD with a BSD userland
              - PHP_OS_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
*/
#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#     define PHP_OS_DARWIN
#     define PHP_OS_BSD4
#     ifdef __LP64__
#        define PHP_OS_DARWIN64
#     else
#        define PHP_OS_QARWIN32
#     endif
#     define PHP_OS_MACOS
#  else
#     error "php++ 还没有移植到这个Apple平台"
#  endif
#elif defined(__sun) || defined(sun)
#  define PHP_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define PHP_OS_HPUX
#elif defined(__linux__) || defined(__linux)
#  define PHP_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  ifndef __FreeBSD_kernel__
#     define PHP_OS_FREEBSD
#  endif
#  define PHP_OS_FREEBSD_KERNEL
#  define PHP_OS_BSD4
#elif defined(__NetBSD__)
#  define PHP_OS_NETBSD
#  define PHP_OS_BSD4
#elif defined(__OpenBSD__)
#  define PHP_OS_OPENBSD
#  define PHP_OS_BSD4
#elif defined(__GNU__)
#  define PHP_OS_HURD
#elif defined(__svr4__) && defined(i386)
#  define PHP_OS_UNIXWARE
#else
#  error "php++ 还没有移植到当前操作系统"
#endif //defined(__APPLE__) && (defined(__GCC__) || defined(__xlC__) || defined(__xlc__))

// 为了兼容定义几个别名
#ifdef PHP_OS_DARWIN
#  define PHP_OS_MAC
#endif
#ifdef PHP_OS_DARWIN32
#  define PHP_OS_MAC32
#endif
#ifdef PHP_OS_DARWIN64
#  define PHP_OS_MAC64
#endif
#ifdef PHP_OS_MACOS
#  define PHP_OS_MACX
#  define PHP_OS_OSX
#endif

#ifdef PHP_OS_DARWIN
#  include <Availability.h>
#  include <AvailabilityMacros.h>

#endif

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#     error "Linux Standard Base版本过低"
#  endif
#endif

#ifndef PHP_LINUX_BASE
#  define PHP_LINUX_BASE
#endif

#endif //PHP_KERNEL_BASE_SYSTEM_DETECTION_H