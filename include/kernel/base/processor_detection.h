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
#ifndef PHP_KERNEL_BASE_PROCESSOR_DETECTION_H
#define PHP_KERNEL_BASE_PROCESSOR_DETECTION_H

/*
    This file uses preprocessor #defines to set various PHP_PROCESSOR_* #defines
    based on the following patterns:

    PHP_PROCESSOR_{FAMILY}
    PHP_PROCESSOR_{FAMILY}_{VARIANT}
    PHP_PROCESSOR_{FAMILY}_{REVISION}

    The first is always defined. Defines for the various revisions/variants are
    optional and usually dependent on how the compiler was invoked. Variants
    that are a superset of another should have a define for the superset.

    In addition to the procesor family, variants, and revisions, we also set
    PHP_BYTE_ORDER appropriately for the target processor. For bi-endian
    processors, we try to auto-detect the byte order using the __BIG_ENDIAN__,
    __LITTLE_ENDIAN__, or __BYTE_ORDER__ preprocessor macros.
*/

//本机byte-order，优先使用预处理器提供的宏定义
#if defined(__ORDER_BIG_ENDIAN__)
#  define PHP_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#  else
#  define PHP_BIG_ENDIAN 4321
#endif

#if defined(__ORDER_LITTLE_ENDIAN__)
#  define PHP_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
#  define PHP_LITTLE_ENDIAN 1234
#endif

/*
    Alpha family, no revisions or variants

    Alpha is bi-endian, use endianness auto-detection implemented below.
*/
#if defined(__alpha__) || defined(_M_ALPHA)
#  define PHP_PROCESSOR_ALPHA
#endif

/*
    ARM family, known revisions: V5, V6, V7, V8

    ARM is bi-endian, detect using __ARMEL__ or __ARMEB__, falling back to
    auto-detection implemented below.
*/
#if defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(__aarch64__)
#  if defined(__aarch64__)
#     define PHP_PROCESSOR_ARM_64
#     define PHP_PROCESSOR_WORDSIZE 8
#  else
#     define PHP_PROCESSOR_ARM_32
#  endif
#  if defined(__ARM_ARCH) && __ARM_ARCH > 1
#     define PHP_PROCESSOR_ARM __ARM_ARCH
#  elif defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM > 1
#     define PHP_PROCESSOR_ARM __TARGET_ARCH_ARM
#  elif defined(_M_ARM) && _M_ARM > 1
#     define PHP_PROCESSOR_ARM _M_ARM
#  elif defined(__ARM64_ARCH_8__) || defined(__aarch64__)
#     define PHP_PROCESSOR_ARM 8
#  elif  defined(__ARM_ARCH_7__) \
      || defined(__ARM_ARCH_7A__) \
      || defined(__ARM_ARCH_7R__) \
      || defined(__ARM_ARCH_7M__) \
      || defined(__ARM_ARCH_7S__) \
      || defined(_ARM_ARCH_7)
#     define PHP_PROCESSOR_ARM 7
#  elif  defined(__ARM_ARCH_6__) \
      || defined(__ARM_ARCH_6J__) \
      || defined(__ARM_ARCH_6T2__) \
      || defined(__ARM_ARCH_6Z__) \
      || defined(__ARM_ARCH_6K__) \
      || defined(__ARM_ARCH_6ZK__) \
      || defined(__ARM_ARCH_6M__)
#     define PHP_PROCESSOR_ARM 6
#  elif defined(__ARM_ARCH_5TEJ__)
#     define PHP_PROCESSOR_ARM 5
#  else 
#     define PHP_PROCESSOR_ARM 0
#  endif
#  if PHP_PROCESSOR_ARM >= 8
#     define PHP_PROCESSOR_ARM_V8
#  endif
#  if PHP_PROCESSOR_ARM >= 7
#     define PHP_PROCESSOR_ARM_V7
#  elif PHP_PROCESSOR_ARM >= 6
#     define PHP_PROCESSOR_ARM_V6
#  elif PHP_PROCESSOR_ARM >= 5
#     define PHP_PROCESSOR_ARM_V5
#  else
#     error "ARM架构太老，我们不支持这个架构的ARM了"
#  endif
#  if defined(__ARMEL__)
#     define PHP_BYTE_ORDER PHP_LITTLE_ENDIAN
#  elif defined(__ARMEB__)
#     define PHP_BYTE_ORDER PHP_BIG_ENDIAN
#  else
// PHP_BYTE_ORDER not defined, use endianness auto-detection
#  endif

/*
    AVR32 family, no revisions or variants

    AVR32 is big-endian.
*/
// #elif defined(__avr32__)
// #  define PHP_PROCESSOR_AVR32
// #  define PHP_BYTE_ORDER PHP_BIG_ENDIAN

/*
    Blackfin family, no revisions or variants

    Blackfin is little-endian.
*/
// #elif defined(__bfin__)
// #  define PHP_PROCESSOR_BLACKFIN
// #  define PHP_BYTE_ORDER PHP_LITTLE_ENDIAN

/*
    X86 family, known variants: 32- and 64-bit

    X86 is little-endian.
*/
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#  define PHP_PROCESSOR_X86_32
#  define PHP_BYTE_ORDER PHP_LITTLE_ENDIAN
#  define PHP_PROCESSOR_WORDSIZE 4

/*
 * We define PHP_PROCESSOR_X86 == 6 for anything above a equivalent or better
 * than a Pentium Pro (the processor whose architecture was called P6) or an
 * Athlon.
 *
 * All processors since the Pentium III and the Athlon 4 have SSE support, so
 * we use that to detect. That leaves the original Athlon, Pentium Pro and
 * Pentium II.
 */
#  if defines(_M_IX86)
#     define PHP_PROCESSOR_X86 (_M_IX86/100)
#  elif defined(__i686__) || defined(__athlon__) || defined(__SSE__) || defined(__pentiumpro__)
#     define PHP_PROCESSOR_X86 6
#  elif defined(__i586__) || defined(__k6__) || defined(__pentium__)
#     define PHP_PROCESSOR_X86 5
#  elif defined(__i486__) || defined(__80486__)
#     define PHP_PROCESSOR_X86 4
#  else
#     define PHP_PROCESSOR_X86 3
#  endif

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
#  define PHP_PROCESSOR_X86 6
#  define PHP_PROCESSOR_X86_64
#  define PHP_BYTE_ORDER PHP_LITTLE_ENDIAN
#  define PHP_PROCESSOR_WORDSIZE 8

/*
    Itanium (IA-64) family, no revisions or variants

    Itanium is bi-endian, use endianness auto-detection implemented below.
*/
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
#  define PHP_PROCESSOR_IA64
#  define PHP_PROCESSOR_WORDSIZE 8

// Q_BYTE_ORDER not defined, use endianness auto-detection

/*
    MIPS family, known revisions: I, II, III, IV, 32, 64

    MIPS is bi-endian, use endianness auto-detection implemented below.
*/
#elif defined(__mips) || defined(__mips__) || defined(_M_MRX000)
#  define PHP_PROCESSOR_MIPS
#  if defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1)
#     define PHP_PROCESSOR_MIPS_I
#  endif
#  if defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2)
#     define PHP_PROCESSOR_MIPS_II
#  endif
#  if defined(_MIPS_ARCH_MIPS32) || defined(__mips32)
#     define PHP_PROCESSOR_MIPS_32
#  endif
#  if defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3)
#     define PHP_PROCESSOR_MIPS_III
#  endif
#  if defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4)
#     define PHP_PROCESSOR_MIPS_IV
#  endif
#  if defined(_MIPS_ARCH_MIPS5) || (defined(__mips) && __mips - 0 >= 5)
#     define PHP_PROCESSOR_MIPS_V
#  endif
#  if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
#     define PHP_PROCESSOR_MIPS64
#     define PHP_PROCESSOR_WORDSIZE 8 
#  endif
#  if defined(__MIPSEL__)
#     #define PHP_BYTE_ORDER PHP_LITTLE_ENDIAN
#  elif define(__MIPSEB__)
#     define PHP_BYTE_ORDER PHP_BIG_ENDIAN
#  else
// PHP_BYTE_ORDER not defined, use endianness auto-detection
#  endif

/*
    Power family, known variants: 32- and 64-bit

    There are many more known variants/revisions that we do not handle/detect.
    See http://en.wikipedia.org/wiki/Power_Architecture
    and http://en.wikipedia.org/wiki/File:PowerISA-evolution.svg

    Power is bi-endian, use endianness auto-detection implemented below.
*/
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \
      || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC) \
      || defined(_M_MPPC) || defined(_M_PPC)
#  define PHP_PROCESSOR_POWER
#  if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
#     define PHP_PROCESSOR_POWER_64
#     define PHP_PROCESSOR_WORDSIZE 8
#  else
#     define PHP_PROCESSOR_POWER_32
#  endif
// PHP_BYTE_ORDER not defined, use endianness auto-detection

/*
    S390 family, known variant: S390X (64-bit)

    S390 is big-endian.
*/
#elif defined(__s390__)
#  define PHP_PROCESSOR_S390
#  if defined(__s390x__)
#     define PHP_PROCESSOR_S390_X
#  endif
#  define PHP_BYTE_ORDER PHP_BIG_ENDIAN

/*
    SuperH family, optional revision: SH-4A

    SuperH is bi-endian, use endianness auto-detection implemented below.
*/
 #elif defined(__sh__)
 #  define PHP_PROCESSOR_SH
 #  if defined(__sh4a__)
 #    define PHP_PROCESSOR_SH_4A
 #  endif
// PHP_BYTE_ORDER not defined, use endianness auto-detection

/*
    SPARC family, optional revision: V9

    SPARC is big-endian only prior to V9, while V9 is bi-endian with big-endian
    as the default byte order. Assume all SPARC systems are big-endian.
*/
#elif defined(__sparc__)
#  define PHP_PROCESSOR_SPARC
#  if defined(__sparc_v9__)
#     define PHP_PROCESSOR_SPARC_V9
#  endif
#  if defined(__sparc64__)
#     define PHP_PROCESSOR_SPARC64
#  endif
#  define PHP_BYTE_ORDER PHP_BIG_ENDIAN

#endif // defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(__aarch64__)

/*
  NOTE:
  GCC 4.6 added __BYTE_ORDER__, __ORDER_BIG_ENDIAN__, __ORDER_LITTLE_ENDIAN__
  and __ORDER_PDP_ENDIAN__ in SVN r165881. If you are using GCC 4.6 or newer,
  this code will properly detect your target byte order; if you are not, and
  the __LITTLE_ENDIAN__ or __BIG_ENDIAN__ macros are not defined, then this
  code will fail to detect the target byte order.
*/
// Some processors support either endian format, try to detect which we are using.
#if !defined(PHP_BYTE_ORDER)
#  if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == PHP_BIG_ENDIAN || __BYTE_ORDER__ == PHP_LITTLE_ENDIAN)
#     define PHP_BYTE_ORDER __BYTE_ORDER__
#  elif defined(__BIG_ENDIAN__) || defined(__big_endian__) || defined(_BIG_ENDIAN)
#     define PHP_BYTE_ORDER PHP_BIG_ENDIAN
#  elif defined(__LITTLE_ENDIAN__) || defined(__little_endian__) || defined(_LITTLE_ENDIAN) \
      || defined(WINAPI_FAMILY)
#     define PHP_BYTE_ORDER PHP_LITTLE_ENDIAN
#  endif
#endif

/*
   Size of a pointer and the machine register size. We detect a 64-bit system by:
   * GCC and compatible compilers (Clang, ICC on OS X and Windows) always define
     __SIZEOF_POINTER__. This catches all known cases of ILP32 builds on 64-bit
     processors.
   * Most other Unix compilers define __LP64__ or _LP64 on 64-bit mode
     (Long and Pointer 64-bit)
   * If PHP_PROCESSOR_WORDSIZE was defined above, it's assumed to match the pointer
     size.
   Otherwise, we assume to be 32-bit and then check in qglobal.cpp that it is right.
*/
#if defined(__SIZEOF_POINTER__)
#  define PHP_POINTER_SIZE  __SIZEOF_POINTER__
#elif defined(__LP64__) || defined(_LP64)
#  define PHP_POINTER_SIZE 8
#elif defined(PHP_PROCESSOR_WORDSIZE)
#  define PHP_POINTER_SIZE PHP_PROCESSOR_WORDSIZE
#else
#  define PHP_POINTER_SIZE 4
#endif

/*
   Define PHP_PROCESSOR_WORDSIZE to be the size of the machine's word (usually,
   the size of the register). On some architectures where a pointer could be
   smaller than the register, the macro is defined above.

   Falls back to PHP_POINTER_SIZE if not set explicitly for the platform.
*/
#ifndef PHP_PROCESSOR_WORDSIZE
#  define PHP_PROCESSOR_WORDSIZE PHP_POINTER_SIZE
#endif

#endif // PHP_KERNEL_BASE_PROCESSOR_DETECTION_H