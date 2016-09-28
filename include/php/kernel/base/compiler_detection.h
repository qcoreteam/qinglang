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
#ifndef PHP_KERNEL_BASE_COMPILER_DETECTION_H
#define PHP_KERNEL_BASE_COMPILER_DETECTION_H

/*
   The compiler, must be one of: (PHP_CC_x)

     SYM      - Digital Mars C/C++ (used to be Symantec C++)
     MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
     BOR      - Borland/Turbo C++
     WAT      - Watcom C++
     GNU      - GNU C++
     COMEAU   - Comeau C++
     EDG      - Edison Design Group C++
     OC       - CenterLine C++
     SUN      - Forte Developer, or Sun Studio C++
     MIPS     - MIPSpro C++
     DEC      - DEC C++
     HPACC    - HP aC++
     USLC     - SCO OUDK and UDK
     CDS      - Reliant C++
     KAI      - KAI C++
     INTEL    - Intel C++ for Linux, Intel C++ for Windows
     HIGHC    - MetaWare High C/C++
     PGI      - Portland Group C++
     GHS      - Green Hills Optimizing C++ Compilers
     RVCT     - ARM Realview Compiler Suite
     CLANG    - C++ front-end for the LLVM compiler
   Should be sorted most to least authoritative.
*/
/* Symantec C++ is now Digital Mars */
#if defined(__DMC__) || defined(__SC__)
#  define PHP_CC_SYM
// "explicit" semantics implemented in 8.1e but keyword recognized since 7.5
#  if defined(__SC__) && __SC__ < 0x750
#     error "当前编译器不支持"
#  endif
#elif defined(_MSC_VER)
#  if defined(__clang__)
#     define PHP_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#  endif
#  define PHP_CC_MSVC (_MSC_VER)
#  define PHP_CC_MSVC_NET
#  define PHP_OUTOFLINE_TEMPLATE inline
#  if _MSC_VER < 1600
#     define PHP_NO_TEMPLATE_FRIENDS
#  endif
#  define PHP_COMPILER_MANGLES_RETURN_TYPE
#  define PHP_FUNC_INFO         __FUNCSIG__
#  define PHP_ALIGNOF(type)     __alignof(type)
#  define PHP_DECL_ALIGN(n)     __declspec(align(n))
#  define PHP_ASSUME_IMPL(expr) __assume(expr)
#  define PHP_UNREACHABLE_IMPL  __assume(0)
#  define PHP_NORETURN          __declspec(noreturn)
#  define PHP_DECL_DEPRECATED   __declspec(deprecated)
#  ifndef PHP_CC_CLANG
#     define PHP_DECL_DEPRECATED_X(text) __declspec(deprecated(text))
#  endif
#  define PHP_DECL_EXPORT __declspec(dllexport)
#  define PHP_DECL_IMPORT __declspec(dllimport)
#  if _MSC_VER >= 1800
#     define PHP_MAKE_UNCHECKED_ARRAY_ITERATOR(x) stdext::make_unchecked_array_iterator(x)
#  endif
#  if _MSC_VER >= 1500
#     define PHP_MAKE_CHECKED_ARRAY_ITERATOR(x, N) stdext::make_checked_array_iterator(x, size_t(N))
#  endif
// Intel C++ disguising as Visual C++: the `using' keyword avoids warnings
#  if defined(__INTEL_COMPILER)
#     define PHP_DECL_VARIABLE_DEPRECATED
#     define PHP_CC_INTEL __INTEL_COMPILER
#  endif
/* only defined for MSVC since that's the only compiler that actually optimizes for this */
/* might get overridden further down when PHP_COMPILER_NOEXCEPT is detected */
#  ifdef __cplusplus
#    define PHP_DECL_NOTHROW  throw()
#  endif

#elif defined(__BORLANDC__) || defined(__TURBOC__)
#  define PHP_CC_BOR
#  define PHP_INLINE_TEMPLATE
#  if __BORLANDC__ < 0x502
#     error "当前编译器不支持"
#  endif

#elif defined(__WATCOMC__)
#  define PHP_CC_WAT

/* ARM Realview Compiler Suite
   RVCT compiler also defines __EDG__ and __GNUC__ (if --gnu flag is given),
   so check for it before that */
#elif defined(__ARMCC__) || defined(__CC_ARM)
#  define PHP_CC_RVCT
// work-around for missing compiler intrinsics
#  define __is_empty(X) false
#  define __is_pod(X) false
#  define PHP_DECL_DEPRECATED __attribute__((__deprecated__))
#  if defined(PHP_OS_LINUX)
#     define PHP_DECL_EXPORT __attribute__((visibility("default")))
#     define PHP_DECL_IMPORT __attribute__((visibility("default")))
#     define PHP_DECL_HIDDEN __attribute__((visibility("hidden")))
#  else
#     define PHP_DECL_EXPORT __declspec(dllexport)
#     define PHP_DECL_IMPORT __declspec(dllimport)
#  endif

#elif defined(__GNUC__)
#  define PHP_CC_GNU (__GNUC__ * 100 + __GNUC_MINOR__)
#  if defined(__MINGW32__)
#     define PHP_CC_MINGW
#  endif
#  if defined(__INTEL_COMPILER)
// Intel C++ also masquerades as GCC
#     define PHP_CC_INTEL (__INTEL_COMPILER)
#     if defined(__clang__)
// Intel C++ masquerades as Clang masquerading as GCC
#        define PHP_CC_CLANG 305
#     endif
#     define PHP_ASSUME_IMPL(expr) __assume(expr)
#     define PHP_UNREACHABLE() __buildin_unreachable()
#     if __INTEL_COMPILER >= 1300 && !defined(__APPLE__)
#        define PHP_DECL_DEPRECATED_X(text) __attribute__((__deprecated__(text)))
#     endif
#  elif defined(__clang__)
// Clang also masquerades as GCC
#     if defined(__apple_build_version__)
// http://en.wikipedia.org/wiki/Xcode#Toolchain_Versions
#        if __apple_build_version__ >= 7000053
#           define PHP_CC_CLANG 306
#        elif __apple_build_version__ >= 6000051
#           define PHP_CC_CLANG 305
#        elif __apple_build_version__ >= 5030038
#           define PHP_CC_CLANG 304
#        elif __apple_build_version__ >= 5000275
#           define PHP_CC_CLANG 303
#        elif __apple_build_version__ >= 4250024
#           define PHP_CC_CLANG 302
#        elif __apple_build_version__ >= 3180045
#           define PHP_CC_CLANG 301
#        elif __apple_build_version__ >= 2111001
#           define PHP_CC_CLANG 300
#        else 
#           error "Unknown Apple Clang version"
#        endif // __apple_build_version__ 
#     else
#        define PHP_CC_CLANG ((__clang_major__ * 100) +  __clang_minor__)
#     endif // defined(__apple_build_version__)
#     if __has_builtin(__builtin_assume)
#        define PHP_ASSUME_IMPL(expr) __builtin_assume(expr)
#     else
#        define PHP_ASSUME_IMPL(expr) if (expr){} else __builtin_unreachable()
#     endif // __has_builtin(__builtin_assume)
#     define PHP_UNREACHABLE_IMPL() __builtin_unreachable()
#     if !defined(__has_extension)
// Compatibility with older Clang versions
#        define __has_extension __has_feature
#     endif
#     if defined(__APPLE__)
// Apple/clang specific features
#        define PHP_DECL_CF_RETURNS_RETAINED __attribute__((cf_returns_retained))
#        if defined (__OBJC__)
#           define PHP_DECL_NS_RETURNS_AUTORELEASED __attribute__((ns_returns_autoreleased))
#        endif
#     endif
// defined(__clang__)
#  else
#     if PHP_CC_GNU >= 405
#        define PHP_ASSUME_IMPL(expr) if (expr){} else __builtin_unreachable()
#        define PHP_UNREACHABLE_IMPL() __builtin_unreachable()
#        define PHP_DECL_DEPRECATED_X(text) __attribute__((__deprecated__(text)))
#     endif
#  endif //defined(__INTEL_COMPILER)

#  if defined(PHP_OS_WIN)
#     define PHP_DECL_EXPORT __declspec(dllexport)
#     define PHP_DECL_IMPORT __declspec(dllimport)
#  elif defined(PHP_VISIBILITY_AVAILABLE)
#     define PHP_DECL_EXPORT __attribute__((visibility("default")))
#     define PHP_DECL_IMPORT __attribute__((visibility("default")))
#     define PHP_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif

#  define PHP_FUNC_INFO               __PRETTY_FUNCTION__
#  define PHP_ALIGNOF(type)           __alignof__(type)
#  define PHP_TYPEOF(expr)            __typeof__(expr)
#  define PHP_DECL_DEPRECATED         __attribute__((__deprecated__))
#  define PHP_DECL_ALIGN(n)           __attribute__((__aligned__(n)))
#  define PHP_DECL_UNUSED             __attribute__((__unused__))
#  define PHP_LIKELY(expr)            __builtin_expect(!!(expr), true)
#  define PHP_UNLIKELY(expr)          __builtin_expect(!!(expr), true)
#  define PHP_NORETURN                __attribute__((__noreturn__))
#  define PHP_REQUIRED_RESULT         __attribute__((__warn_unused_result__))
#  define PHP_DECL_PURE_FUNCTION      __attribute__((pure))
#  define PHP_DECL_CONST_FUNCTION     __attribute__((const))
#  define PHP_PACKED                  __attribute__((__packed__))
#  if !defined(__ARM_EABI__)
#     define PHP_NO_ARM_EABI
#  endif
#  if PHP_CC_GNU >= 403 && !defined(PHP_CC_CLANG)
#     define PHP_ALLOC_SIZE(x) __attribute__((alloc_size(x)))
#  endif

/* IBM compiler versions are a bit messy. There are actually two products:
   the C product, and the C++ product. The C++ compiler is always packaged
   with the latest version of the C compiler. Version numbers do not always
   match. This little table (I'm not sure it's accurate) should be helpful:

   C++ product                C product

   C Set 3.1                  C Compiler 3.0
   ...                        ...
   C++ Compiler 3.6.6         C Compiler 4.3
   ...                        ...
   Visual Age C++ 4.0         ...
   ...                        ...
   Visual Age C++ 5.0         C Compiler 5.0
   ...                        ...
   Visual Age C++ 6.0         C Compiler 6.0

   Now:
   __xlC__    is the version of the C compiler in hexadecimal notation
              is only an approximation of the C++ compiler version
   __IBMCPP__ is the version of the C++ compiler in decimal notation
              but it is not defined on older compilers like C Set 3.1 */
#elif defined(__xlC__)
#  define PHP_CC_XLC
#  define PHP_FULL_TEMPLATE_INSTANTIATION
#  if __xlC < 0x400
#     error "当前编译器不支持"
#  elif __xlC >= 0x600
#     define PHP_ALIGNOF(type)  __alignof__(type)
#     define PHP_TYPEOF(expr)   __typeof__(expr)
#     define PHP_DECL_ALIGN(n)  __attribute__((__aligned__(n)))
#     define PHP_PACKED         __attribute__((__packed__))
#  endif

/* Older versions of DEC C++ do not define __EDG__ or __EDG - observed
   on DEC C++ V5.5-004. New versions do define  __EDG__ - observed on
   Compaq C++ V6.3-002.
   This compiler is different enough from other EDG compilers to handle
   it separately anyway. */
#elif defined(__DECCXX) || defined(__DECC)
#  define PHP_CC_DEC
/* Compaq C++ V6 compilers are EDG-based but I'm not sure about older
   DEC C++ V5 compilers.*/
#  if defined(__EDG__)
#     define PHP_CC_DEG
#  endif

/* Compaq has disabled EDG's _BOOL macro and uses _BOOL_EXISTS instead
   - observed on Compaq C++ V6.3-002.
   In any case versions prior to Compaq C++ V6.0-005 do not have bool. */
#  if !defined(__BOOL_EXISTS)
#     error "当前编译器不支持"
#  endif

/* Spurious (?) error messages observed on Compaq C++ V6.5-014. */
/* Apply to all versions prior to Compaq C++ V6.0-000 - observed on
   DEC C++ V5.5-004. */
#  if __DECCXX < 60060000
#     define PHP_BROKEN_TEMPLATE_SPECIALIZATION
#  endif
//avoid undefined symbol problems with out-of-line template members
#  define PHP_OUTOFLINE_TEMPLATE inline

/* The Portland Group C++ compiler is based on EDG and does define __EDG__
   but the C compiler does not */
#elif defined(__PGI)
#  define PHP_CC_PGI
#  if defined(__EDG__)
#     define PHP_CC_EDG
#  endif

/* Compilers with EDG front end are similar. To detect them we test:
   __EDG documented by SGI, observed on MIPSpro 7.3.1.1 and KAI C++ 4.0b
   __EDG__ documented in EDG online docs, observed on Compaq C++ V6.3-002
   and PGI C++ 5.2-4 */
#elif !defined(PHP_OS_HPUX) && (defined(__EDG) || defined(__EDG__))
#  define PHP_CC_EDG

/* From the EDG documentation (does not seem to apply to Compaq C++ or GHS C):
   _BOOL
        Defined in C++ mode when bool is a keyword. The name of this
        predefined macro is specified by a configuration flag. _BOOL
        is the default.
   __BOOL_DEFINED
        Defined in Microsoft C++ mode when bool is a keyword. */

#  if !defined(_BOOL) && !defined(__BOOL_DEFINED) && !defined(__ghs)
#     error "当前编译器不支持"
#  endif
// The Comeau compiler is based on EDG and does define __EDG__
#  if defined(__COMO__)
#     define PHP_CC_COMEAU
/* The `using' keyword was introduced to avoid KAI C++ warnings
   but it's now causing KAI C++ errors instead. The standard is
   unclear about the use of this keyword, and in practice every
   compiler is using its own set of rules. Forget it. */
#  elif defined(__KCC)
#     define PHP_CC_KAI
// Using the `using' keyword avoids Intel C++ for Linux warnings
#  elif defined(__INTEL_COMPILER)
#     define PHP_CC_INTEL (__INTEL_COMPILER)
// Uses CFront, make sure to read the manual how to tweak templates.
#  elif defined(__ghs)
#     define PHP_CC_GHS
#     define PHP_DECL_DEPRECATED      __attribute__((__deprecated__))
#     define PHP_FUNC_INFO            __PRETTY_FUNCTION__
#     define PHP_TYPEOF(expr)         __typeof__(expr)
#     define PHP_ALIGNOF(type)        __alignof__(type)
#     define PHP_UNREACHABLE_IMPL()   
#     if defined(__cplusplus)
#        define PHP_COMPILER_AUTO_TYPE
#        define PHP_COMPILER_STATIC_ASSERT
#        define PHP_COMPILER_RANGE_FOR
#        if __GHS_VERSION_NUMBER >= 201505
#           define PHP_COMPILER_ALIGNAS
#           define PHP_COMPILER_ALIGNOF
#           define PHP_COMPILER_ATOMICS
#           define PHP_COMPILER_ATTRIBUTES
#           define PHP_COMPILER_AUTO_FUNCTION
#           define PHP_COMPILER_CLASS_ENUM
#           define PHP_COMPILER_CONSTEXPR
#           define PHP_COMPILER_DECLTYPE
#           define PHP_COMPILER_DEFAULT_MEMBERS
#           define PHP_COMPILER_DELETE_MEMBERS
#           define PHP_COMPILER_DELEGATING_CONSTRUCTORS
#           define PHP_COMPILER_EXPLICIT_CONVERSIONS
#           define PHP_COMPILER_EXPLICIT_OVERRIDES
#           define PHP_COMPILER_EXTERN_TEMPLATES
#           define PHP_COMPILER_INHERITING_CONSTRUCTORS
#           define PHP_COMPILER_INITIALIZER_LISTS
#           define PHP_COMPILER_LAMBDA
#           define PHP_COMPILER_NONSTATIC_MEMBER_INIT
#           define PHP_COMPILER_NOEXCEPT
#           define PHP_COMPILER_NULLPTR
#           define PHP_COMPILER_RANGE_FOR
#           define PHP_COMPILER_RAW_STRINGS
#           define PHP_COMPILER_REF_QUALIFIERS
#           define PHP_COMPILER_RVALUE_REFS
#           define PHP_COMPILER_STATIC_ASSERT
#           define PHP_COMPILER_TEMPLATE_ALIAS
#           define PHP_COMPILER_THREAD_LOCAL
#           define PHP_COMPILER_THREADSAFE_STATICS
#           define PHP_COMPILER_UDL
#           define PHP_COMPILER_UNICODE_STRINGS
#           define PHP_COMPILER_UNIFORM_INIT
#           define PHP_COMPILER_UNRESTRICTED_UNIONS
#           define PHP_COMPILER_VARIADIC_MACROS
#           define PHP_COMPILER_VARIADIC_TEMPLATES
#        endif // __GHS_VERSION_NUMBER >= 201505
#     endif // defined(__cplusplus)

#  elif defined(__DCC__)
#     define PHP_CC_DIAB
#     if !defined(__bool)
#        error "当前编译器不支持"
#     endif
// The UnixWare 7 UDK compiler is based on EDG and does define __EDG__
#  elif defined(__USLC__) && defined(__SCO_VERSION__)
#     define PHP_CC_USLC
// The latest UDK 7.1.1b does not need this, but previous versions do
#     if !defined(__SCO_VERSION__) || (__SCO_VERSION < 302200010)
#        define PHP_OUTOFLINE_TEMPLATE inline
#     endif
// Never tested!
#  elif define(CENTERLINE_CLPP) || defined(OBJECTCENTER)
#     define PHP_CC_OC

/* CDS++ defines __EDG__ although this is not documented in the Reliant
   documentation. It also follows conventions like _BOOL and this documented */
#  elif defined(sinix)
#     define PHP_CC_CDS

// The MIPSpro compiler defines __EDG
#  elif defined(__sgi)
#     define PHP_CC_MIPS
#     define PHP_NO_TEMPLATE_FRIENDS
#     if defined(_COMPILER_VERSION) && (_COMPILER_VERSION >= 740)
#        define PHP_OUTOFLINE_TEMPLATE inline
#        pragma set woff 3624, 3625, 3649 // turn off some harmless warnings
#     endif
#  endif // defined(__COMO__)

/* VxWorks' DIAB toolchain has an additional EDG type C++ compiler
   (see __DCC__ above). This one is for C mode files (__EDG is not defined) */
#elif defined(_DIAB_TOOL)
#  define PHP_CC_DIAB
#  define PHP_FUNC_INFO __PRETTY_FUNCTION__

// Never tested!
#elif defined(__HIGHC__)
#  define PHP_CC_HIGHC

#elif defined(__SUNPRO__CC) || defined(__SUNPRO_C)
#  define PHP_CC_SUM
#  define PHP_COMPILER_MANGLES_RETURN_TYPE
/* 5.0 compiler or better
    'bool' is enabled by default but can be disabled using -features=nobool
    in which case _BOOL is not defined
        this is the default in 4.2 compatibility mode triggered by -compat=4 */
#  if __SUNPRO_CC >= 0x500
#     define PHP_NO_TEMPLATE_TEMPLATE_PARAMETERS
// see http://developers.sun.com/sunstudio/support/Ccompare.html
#     if __SUNPRO_CC >= 0x590
#        define PHP_ALIGNOF(type)  __alignof__(type)
#        define PHP_TYPEOF(expr)   __typeof__(expr)
#        define PHP_DECL_ALIGN(n)  __attribute__((__aligned__(n)))
#     endif
#     if __SUNPRO_CC >= 0x550
#        define PHP_DECL_EXPORT __global
#     endif
#     if __SUNPRO_CC <= 0x5a0
#        define PHP_NO_TEMPLATE_FRIENDS
#     endif
#     if !defined(__BOOL)
#        error "当前编译器不支持"
#     endif
#  else
#     error "当前编译器不支持"
#  endif

/* CDS++ does not seem to define __EDG__ or __EDG according to Reliant
   documentation but nevertheless uses EDG conventions like _BOOL */
#elif defined(sinix)
#  define PHP_CC_EDG
#  define PHP_CC_CDS
#  if !defined(_BOOL)
#     error "当前编译器不支持"
#  endif
#  define PHP_BROKEN_TEMPLATE_SPECIALIZATION

#elif defined(PHP_OS_HPUX)
// __HP_aCC was not defined in first aCC releases
#  if defined(__HP_aCC) || __cplusplus > 199707L
#     define PHP_NO_TEMPLATE_FRIENDS
#     define PHP_CC_HPACC
#     define PHP_FUNC_INFO __PRETTY_FUNCTION__
#     if __HP_aCC-0 < 060000
#        define PHP_NO_TEMPLATE_TEMPLATE_PARAMETERS
#        define PHP_DECL_EXPORT __declspec(dllexport)
#        define PHP_DECL_IMPORT __declspec(dllimport)
#     endif
#     if __HP_aCC-0 >= 061200
#        define PHP_DECL_ALIGN(n) __attribute__((aligned(n)))
#     endif
#     if __HP_aCC-0 >= 062000
#        define PHP_DECL_EXPORT __attribute__((visibility("default")))
#        define PHP_DECL_HIDDEN __attribute__((visibility("hidden")))
#        define PHP_DECL_IMPORT PHP_DECL_EXPORT 
#     endif
#  else
#  error "当前编译器不支持"
#  endif
#else
#  error "php++ has not been tested with this compiler"
#endif // defined(__DMC__) || defined(__SC__) 

/*
 * C++11 support
 *
 *  Paper           Macro                               SD-6 macro
 *  N2341           PHP_COMPILER_ALIGNAS
 *  N2341           PHP_COMPILER_ALIGNOF
 *  N2427           PHP_COMPILER_ATOMICS
 *  N2761           PHP_COMPILER_ATTRIBUTES               __cpp_attributes = 200809
 *  N2541           PHP_COMPILER_AUTO_FUNCTION
 *  N1984 N2546     PHP_COMPILER_AUTO_TYPE
 *  N2437           PHP_COMPILER_CLASS_ENUM
 *  N2235           PHP_COMPILER_CONSTEXPR                __cpp_constexpr = 200704
 *  N2343 N3276     PHP_COMPILER_DECLTYPE                 __cpp_decltype = 200707
 *  N2346           PHP_COMPILER_DEFAULT_MEMBERS
 *  N2346           PHP_COMPILER_DELETE_MEMBERS
 *  N1986           PHP_COMPILER_DELEGATING_CONSTRUCTORS
 *  N2437           PHP_COMPILER_EXPLICIT_CONVERSIONS
 *  N3206 N3272     PHP_COMPILER_EXPLICIT_OVERRIDES
 *  N1987           PHP_COMPILER_EXTERN_TEMPLATES
 *  N2540           PHP_COMPILER_INHERITING_CONSTRUCTORS
 *  N2672           PHP_COMPILER_INITIALIZER_LISTS
 *  N2658 N2927     PHP_COMPILER_LAMBDA                   __cpp_lambdas = 200907
 *  N2756           PHP_COMPILER_NONSTATIC_MEMBER_INIT
 *  N2855 N3050     PHP_COMPILER_NOEXCEPT
 *  N2431           PHP_COMPILER_NULLPTR
 *  N2930           PHP_COMPILER_RANGE_FOR
 *  N2442           PHP_COMPILER_RAW_STRINGS              __cpp_raw_strings = 200710
 *  N2439           PHP_COMPILER_REF_QUALIFIERS
 *  N2118 N2844 N3053 PHP_COMPILER_RVALUE_REFS            __cpp_rvalue_references = 200610
 *  N1720           PHP_COMPILER_STATIC_ASSERT            __cpp_static_assert = 200410
 *  N2258           PHP_COMPILER_TEMPLATE_ALIAS
 *  N2659           PHP_COMPILER_THREAD_LOCAL
 *  N2660           PHP_COMPILER_THREADSAFE_STATICS
 *  N2765           PHP_COMPILER_UDL                      __cpp_user_defined_literals = 200809
 *  N2442           PHP_COMPILER_UNICODE_STRINGS          __cpp_unicode_literals = 200710
 *  N2640           PHP_COMPILER_UNIFORM_INIT
 *  N2544           PHP_COMPILER_UNRESTRICTED_UNIONS
 *  N1653           PHP_COMPILER_VARIADIC_MACROS
 *  N2242 N2555     PHP_COMPILER_VARIADIC_TEMPLATES       __cpp_variadic_templates = 200704
 *
 * For any future version of the C++ standard, we use only the SD-6 macro.
 * For full listing, see
 *  http://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
 *
 * C++ extensions:
 *  PHP_COMPILER_RESTRICTED_VLA       variable-length arrays, prior to __cpp_runtime_arrays
 */
#if defined(__cplusplus)
#  if __cplusplus < 201103L && !(defined(PHP_CC_MSVC) && PHP_CC_MSVC >= 1800)
#      error "php++ requires a C++11 compiler and yours does not seem to be that."
#  endif
#endif // defined(__cplusplus)

#if defined(PHP_CC_INTEL)
#  define PHP_COMPILER_RESTRICTED_VLA
#  define PHP_COMPILER_VARIADIC_MACROS
#  define PHP_COMPILER_THREADSAFE_STATICS
#  if __INTEL_COMPILER < 1200
#     define PHP_NO_TEMPLATE_FRIENDS
#  endif
#  if __INTEL_COMPILER >= 1300 && !defined(_WIN32)
//    ICC supports C++14 binary literals in C, C++98, and C++11 modes
//    at least since 13.1, but I can't test further back
#     define PHP_COMPILER_BINARY_LITERALS
#  endif
#  if __cplusplus >= 201103L || defined(__INTEL_CXX11_MODE__)
#     if __INTEL_COMPILER >= 1200
#        define PHP_COMPILER_AUTO_TYPE
#        define PHP_COMPILER_CLASS_ENUM
#        define PHP_COMPILER_DECLTYPE
#        define PHP_COMPILER_DEFAULT_MEMBERS
#        define PHP_COMPILER_DELETE_MEMBERS
#        define PHP_COMPILER_EXTERN_TEMPLATES
#        define PHP_COMPILER_LAMBDA
#        define PHP_COMPILER_RVALUE_REFS
#        define PHP_COMPILER_STATIC_ASSERT
#        define PHP_COMPILER_VARIADIC_MACROS
#     endif // __INTEL_COMPILER >= 1200
#     if __INTEL_COMPILER > 1210
#        define PHP_COMPILER_ATTRIBUTES
#        define PHP_COMPILER_AUTO_FUNCTION
#        define PHP_COMPILER_NULLPTR
#        define PHP_COMPILER_TEMPLATE_ALIAS
#        if !defined(_CHAR16T) // msvc headers
#           define PHP_COMPILER_UNICODE_STRINGS
#        endif
#        define PHP_COMPILER_VARIADIC_TEMPLATES
#     endif // __INTEL_COMPILER > 1210
#     if __INTEL_COMPILER > 1300
//       constexpr support is only partial
//#      define PHP_COMPILER_CONSTEXPR
#        define PHP_COMPILER_INITIALIZER_LISTS
#        define PHP_COMPILER_UNIFORM_INIT
#        define PHP_COMPILER_NOEXCEPT
#     endif // __INTEL_COMPILER > 1300
#     if __INTEL_COMPILER > 1400
//       Intel issue ID 6000056211, bug DPD200534796
//#      define Q_COMPILER_CONSTEXPR
#        define PHP_COMPILER_DELEGATING_CONSTRUCTORS
#        define PHP_COMPILER_EXPLICIT_CONVERSIONS
#        define PHP_COMPILER_EXPLICIT_OVERRIDES
#        define PHP_COMPILER_NONSTATIC_MEMBER_INIT
#        define PHP_COMPILER_RANGE_FOR
#        define PHP_COMPILER_RAW_STRINGS
#        define PHP_COMPILER_REF_QUALIFIERS
#        define PHP_COMPILER_UNICODE_STRINGS
#        define PHP_COMPILER_UNRESTRICTED_UNIONS
#     endif // __INTEL_COMPILER > 1400
#     if __INTEL_COMPILER > 1500
#        if __INTEL_COMPILER * 100 + __INTEL_COMPILER_UPDATE >= 150001
//       the bug mentioned above is fixed in 15.0.1
#           define PHP_COMPILER_CONSTEXPR
#        endif
#        define PHP_COMPILER_ALIGNAS
#        define PHP_COMPILER_ALIGNOF
#        define PHP_COMPILER_INHERITING_CONSTRUCTORS
#        if !defined(PHP_OS_OSX)
//       C++11 thread_local is broken on OS X (Clang doesn't support it either)
#           define PHP_COMPILER_THREAD_LOCAL
#        endif
#        define PHP_COMPILER_UDL
#     endif
#     if defined(_MSC_VER)
#        if _MSC_VER == 1700
//       <initializer_list> is missing with MSVC 2012 (it's present in 2010, 2013 and up)
#           undef PHP_COMPILER_INITIALIZER_LISTS      
#        endif
#        if _MSC_VER < 1900
//       ICC disables unicode string support when compatibility mode with MSVC 2013 or lower is active
#           undef PHP_COMPILER_UNICODE_STRINGS
//       Even though ICC knows about ref-qualified members, MSVC 2013 or lower doesn't, so
//       certain member functions may be missing from the DLLs.
#           undef PHP_COMPILER_REF_QUALIFIERS
//       Disable constexpr unless the MS headers have constexpr in all the right places too
//       (like std::numeric_limits<T>::max())
#           undef PHP_COMPILER_CONSTEXPR
#        endif
#     endif
#  endif // __cplusplus >= 201103L || defined(__INTEL_CXX11_MODE__)
#endif // defined(PHP_CC_INTEL)

#if defined(PHP_CC_CLANG) && !defined(PHP_CC_INTEL)
// General C++ features
#  define PHP_COMPILER_RESTRICTED_VLA
#  define PHP_COMPILER_THREADSAFE_STATICS
#  if __has_feature(attribute_deprecated_with_message)
#     define PHP_DECL_DEPRECATED_X(text) __attribute__((__deprecated__(text)))
#  endif

// Clang supports binary literals in C, C++98 and C++11 modes
// It's been supported "since the dawn of time itself" (cf. commit 179883)
#  if __has_extension(cxx_binary_literals)
#     define PHP_COMPILER_BINARY_LITERALS
#  endif

// Variadic macros are supported for gnu++98, c++11, c99 ... since 2.9
#  if PHP_CC_CLANG >= 209
#     if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
         || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
         || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
#        define PHP_COMPILER_VARIADIC_MACROS
#     endif
#  endif

/* C++11 features, see http://clang.llvm.org/cxx_status.html */
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
// Detect C++ features using __has_feature(), see http://clang.llvm.org/docs/LanguageExtensions.html#cxx11
#     if __has_feature(cxx_alignas)
#        define PHP_COMPILER_ALIGNAS
#        define PHP_COMPILER_ALIGNOF
#     endif
#     if __has_feature(cxx_atomic) && __has_include(<atomic>)
#        define PHP_COMPILER_ATOMICS
#     endif
#     if __has_feature(cxx_attributes)
#        define PHP_COMPILER_ATTRIBUTES
#     endif
#     if __has_feature(cxx_auto_type)
#        define PHP_COMPILER_AUTO_FUNCTION
#        define PHP_COMPILER_AUTO_TYPE
#     endif
#     if __has_feature(cxx_strong_enums)
#        define PHP_COMPILER_CLASS_ENUM
#     endif
// CLANG 3.2 has bad/partial support
#     if __has_feature(cxx_constexpr) && PHP_CC_CLANG > 302
#        define PHP_COMPILER_CONSTREXPR
#     endif
#     if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#        define PHP_COMPILER_DECLTYPE
#     endif
#     if __has_feature(cxx_defaulted_functions)
#        define PHP_COMPILER_DEFAULT_MEMBERS
#     endif
#     if __has_feature(cxx_deleted_functions)
#        define PHP_COMPILER_DELETE_MEMBERS
#     endif
#     if __has_feature(cxx_delegating_constructors)
#        define PHP_COMPILER_DELEGATING_CONSTRUCTORS
#     endif
#     if __has_feature(cxx_explicit_conversions)
#        define PHP_COMPILER_EXPLICIT_CONVERSIONS
#     endif
#     if __has_feature(cxx_override_control)
#        define PHP_COMPILER_EXPLICIT_OVERRIDES
#     endif
#     if __has_feature(cxx_inheriting_constructors)
#        define PHP_COMPILER_INHERITING_CONSTRUCTORS
#     endif
#     if __has_feature(cxx_generalized_initializers)
#        define PHP_COMPILER_INITIALIZER_LISTS
#        define PHP_COMPILER_UNIFORM_INIT // both covered by this feature macro, according to docs
#     endif
#     if __has_feature(cxx_lambdas)
#        define PHP_COMPILER_LAMBDA
#     endif
#     if __has_feature(cxx_noexcept)
#        define PHP_COMPILER_NOEXCEPT
#     endif
#     if __has_feature(cxx_nonstatic_member_init)
#        define PHP_COMPILER_NONSTATIC_MEMBER_INIT
#     endif
#     if __has_feature(cxx_nullptr)
#        define PHP_COMPILER_NULLPTR
#     endif
#     if __has_feature(cxx_range_for)
#        define PHP_COMPILER_RANGE_FOR
#     endif
#     if __has_feature(cxx_raw_string_literals)
#        define PHP_COMPILER_RAW_STRINGS
#     endif
#     if __has_feature(cxx_reference_qualified_functions)
#        define PHP_COMPILER_REF_QUALIFIERS
#     endif
#     if __has_feature(cxx_rvalue_references)
#        define PHP_COMPILER_RVALUE_REFS
#     endif
#     if __has_feature(cxx_static_assert)
#        define PHP_COMPILER_STATIC_ASSERT
#     endif
#     if __has_feature(cxx_alias_templates)
#        define PHP_COMPILER_TEMPLATE_ALIAS
#     endif
#     if __has_feature(cxx_thread_local)
#        if !defined(__FreeBSD__)
#           define PHP_COMPILER_THREAD_LOCAL
#        endif
#     endif
#     if __has_feature(cxx_user_literals)
#        define PHP_COMPILER_UDL
#     endif
#     if __has_feature(cxx_unicode_literals)
#        define PHP_COMPILER_UNICODE_STRINGS
#     endif
#     if __has_feature(cxx_unrestricted_unions)
#        define PHP_COMPILER_UNRESTRICTED_UNIONS
#     endif
#     if __has_feature(cxx_variadic_templates)
#        define PHP_COMPILER_VARIADIC_TEMPLATES
#     endif
// Features that have no __has_feature() check
#     if PHP_CC_CLANG >= 209 /*since clang 2.9 */
#        define PHP_COMPILER_EXTERN_TEMPLATES
#     endif
#  endif
/* C++1y features, deprecated macros. Do not update this list. */
#  if __cplusplus > 201103L
//#    if __has_feature(cxx_binary_literals)
//#      define PHP_COMPILER_BINARY_LITERALS  // see above
//#    endif
#     if __has_feature(cxx_generic_lambda)
#        define PHP_COMPILER_GENERIC_LAMBDA

#     endif
#     if __has_feature(cxx_init_capture)
#        define PHP_COMPILER_LAMBDA_CAPTURES
#     endif
#     if __has_feature(cxx_relaxed_constexpr)
#        define PHP_COMPILER_RELAXED_CONSTEXPR_FUNCTIONS
#     endif
#     if __has_feature(cxx_decltype_auto) && __has_feature(cxx_return_type_deduction)
#        define PHP_COMPILER_RETURN_TYPE_DEDUCTION
#     endif
#     if __has_feature(cxx_variadic_templates)
#        define PHP_COMPILER_VARIADIC_TEMPLATES
#     endif
#     if __has_feature(cxx_runtime_array)
#        define PHP_COMPILER_VLA
#     endif
#  endif // __cplusplus > 201103L

#  if defined(__has_warning)
#     if __has_warning("-Wunused-private-field")
#        define PHP_DECL_UNUSED_MEMBER PHP_DECL_UNUSED
#     endif
#  endif
#endif // defined(PHP_CC_CLANG) && !defined(PHP_CC_INTEL)

#if defined(PHP_CC_GNU) && !defined(PHP_CC_INTEL) && !defined(PHP_CC_CLANG)
#  define PHP_COMPILER_RESTRICTED_VLA
#  define PHP_COMPILER_THREADSAFE_STATICS
#  if PHP_CC_GNU >= 403
//   GCC supports binary literals in C, C++98 and C++11 modes
#     define PHP_COMPILER_BINARY_LITERALS
#  endif
#  if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
      || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
      || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
// Variadic macros are supported for gnu++98, c++11, C99 ... since forever (gcc 2.97)
#     define PHP_COMPILER_VARIADIC_MACROS
#  endif
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#     if PHP_CC_GNU >= 403
// C++11 features supported in GCC 4.3:
#        define PHP_COMPILER_DECLTYPE
#        define PHP_COMPILER_RVALUE_REFS
#        define PHP_COMPILER_STATIC_ASSERT
#     endif
#     if PHP_CC_GNU >= 404
// C++11 features supported in GCC 4.4:
#        define PHP_COMPILER_AUTO_FUNCTION
#        define PHP_COMPILER_AUTO_TYPE
#        define PHP_COMPILER_EXTERN_TEMPLATES
#        define PHP_COMPILER_UNIFORM_INIT
#        define PHP_COMPILER_UNICODE_STRINGS
#        define PHP_COMPILER_VARIADIC_TEMPLATES
#     endif
#     if PHP_CC_GNU >= 405
// C++11 features supported in GCC 4.5:
#        define PHP_COMPILER_EXPLICIT_CONVERSIONS
/* GCC 4.4 implements initializer_list but does not define typedefs required
  * by the standard. */
#        define PHP_COMPILER_INITIALIZER_LISTS
#        define PHP_COMPILER_LAMBDA
#        define PHP_COMPILER_RAW_STRINGS
#        define PHP_COMPILER_CLASS_ENUM
#     endif
#     if PHP_CC_GNU >= 406
/* Pre-4.6 compilers implement a non-final snapshot of N2346, hence default and delete
  * functions are supported only if they are public. Starting from 4.6, GCC handles
  * final version - the access modifier is not relevant. */
#        define PHP_COMPILER_DEFAULT_MEMBERS
#        define PHP_COMPILER_DELETE_MEMBERS
// C++11 features supported in GCC 4.6:
#        define PHP_COMPILER_CONSTEXPR
#        define PHP_COMPILER_NULLPTR
#        define PHP_COMPILER_UNRESTRICTED_UNIONS
#        define PHP_COMPILER_RANGE_FOR
#     endif
#     if PHP_CC_GNU >= 407
/* GCC 4.4 implemented <atomic> and std::atomic using its old intrinsics.
  * However, the implementation is incomplete for most platforms until GCC 4.7:
  * instead, std::atomic would use an external lock. Since we need an std::atomic
  * that is behavior-compatible with QBasicAtomic, we only enable it here */
#        define PHP_COMPILER_ATOMICS
/* GCC 4.6.x has problems dealing with noexcept expressions,
  * so turn the feature on for 4.7 and above, only */
#        define PHP_COMPILER_NOEXCEPT
// C++11 features supported in GCC 4.7:
#        define PHP_COMPILER_NONSTATIC_MEMBER_INIT
#        define PHP_COMPILER_DELEGATING_CONSTRUCTORS
#        define PHP_COMPILER_EXPLICIT_OVERRIDES
#        define PHP_COMPILER_TEMPLATE_ALIAS
#        define PHP_COMPILER_UDL
#     endif
#     if PHP_CC_GNU >= 408
#        define PHP_COMPILER_ATTRIBUTES
#        define PHP_COMPILER_ALIGNAS
#        define PHP_COMPILER_ALIGNOF
#        define PHP_COMPILER_INHERITING_CONSTRUCTORS
#        define PHP_COMPILER_THREAD_LOCAL
#        if PHP_CC_GNU > 408 || __GNUC_PATCHLEVEL__ >= 1
#           define PHP_COMPILER_REF_QUALIFIERS
#        endif
#     endif
// C++11 features are complete as of GCC 4.8.1
#  endif // defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#  if __cplusplus > 201103L
#     if PHP_CC_GNU >= 409
/* C++1y features in GCC 4.9 - deprecated, do not update this list */
//#    define PHP_COMPILER_BINARY_LITERALS   // already supported since GCC 4.3 as an extension
#        define PHP_COMPILER_LAMBDA_CAPTURES
#        define PHP_COMPILER_RETURN_TYPE_DEDUCTION
#     endif
#  endif // __cplusplus > 201103L
#endif // defiend(PHP_CC_GNU) && !defined(PHP_CC_INTEL) && !defined(PHP_CC_CLANG)

#if defined(PHP_CC_MSVC) && !defined(PHP_CC_INTEL)
#  if defined(__cplusplus)
#     if _MSC_VER >= 1400
// C++11 features supported in VC8 = VC2005:
#        define PHP_COMPILER_VARIADIC_MACROS
#        if !defined(__cplusplus_cli)
/* 2005 supports the override and final contextual keywords, in
 the same positions as the C++11 variants, but 'final' is
 called 'sealed' instead:
 http://msdn.microsoft.com/en-us/library/0w2w91tf%28v=vs.80%29.aspx
 The behavior is slightly different in C++/CLI, which requires the
 "virtual" keyword to be present too, so don't define for that.
 So don't define Q_COMPILER_EXPLICIT_OVERRIDES (since it's not
 the same as the C++11 version), but define the Q_DECL_* flags
 accordingly: */
#           define PHP_DECL_OVERRIDE override
#           define PHP_DECL_FINAL sealed
#        endif
#     endif
#     if _MSC_VER >= 1600
// C++11 features supported in VC10 = VC2010:
#        define PHP_COMPILER_AUTO_FUNCTION
#        define PHP_COMPILER_AUTO_TYPE
#        define PHP_COMPILER_DECLTYPE
#        define PHP_COMPILER_EXTERN_TEMPLATES
#        define PHP_COMPILER_LAMBDA
#        define PHP_COMPILER_NULLPTR
#        define PHP_COMPILER_RVALUE_REFS
#        define PHP_COMPILER_STATIC_ASSERT
//  MSVC's library has std::initializer_list, but the compiler does not support the braces initialization
//#      define PHP_COMPILER_INITIALIZER_LISTS
//#      define PHP_COMPILER_UNIFORM_INIT
#     endif // _MSC_VER >= 1600
#     if _MSC_VER >= 1700
// C++11 features supported in VC11 = VC2012:
#        undef PHP_DECL_OVERRIDE /* undo 2005/2008 settings... */
#        undef PHP_DECL_FINAL  /* undo 2005/2008 settings... */
#        define PHP_COMPILER_EXPLICIT_OVERRIDES /* ...and use std C++11 now   */
#        define PHP_COMPILER_CLASS_ENUM
#        define PHP_COMPILER_ATOMICS
#     endif // VC 11
#     if _MSC_VER >= 1800
/* C++11 features in VC12 = VC2013 */
/* Implemented, but can't be used on move special members */
/* #      define Q_COMPILER_DEFAULT_MEMBERS */
#       define PHP_COMPILER_DELETE_MEMBERS
#       define PHP_COMPILER_DELEGATING_CONSTRUCTORS
#       define PHP_COMPILER_EXPLICIT_CONVERSIONS
#       define PHP_COMPILER_NONSTATIC_MEMBER_INIT
// implemented, but nested initialization fails (eg tst_qvector): http://connect.microsoft.com/VisualStudio/feedback/details/800364/initializer-list-calls-object-destructor-twice
//      #define PHP_COMPILER_INITIALIZER_LISTS
// implemented in principle, but has a bug that makes it unusable: http://connect.microsoft.com/VisualStudio/feedback/details/802058/c-11-unified-initialization-fails-with-c-style-arrays
//      #define PHP_COMPILER_UNIFORM_INIT
#       define PHP_COMPILER_RAW_STRINGS
#       define PHP_COMPILER_TEMPLATE_ALIAS
#       define PHP_COMPILER_VARIADIC_TEMPLATES
#     endif // VC 12
#     if _MSC_FULL_VER >= 180030324 // VC 12 SP 2 RC
#        define PHP_COMPILER_INITIALIZER_LISTS
#     endif // VC 12 SP 2 RC
#     if _MSC_VER >= 1900
// C++11 features in VC14 = VC2015
#        define PHP_COMPILER_DEFAULT_MEMBERS
#        define PHP_COMPILER_ALIGNAS
#        define PHP_COMPILER_ALIGNOF
// Partial support, insufficient for Qt
//#      define PHP_COMPILER_CONSTEXPR
#        define PHP_COMPILER_INHERITING_CONSTRUCTORS
#        define PHP_COMPILER_NOEXCEPT
#        define PHP_COMPILER_RANGE_FOR
#        define PHP_COMPILER_REF_QUALIFIERS
#        define PHP_COMPILER_THREAD_LOCAL
// Broken, see QTBUG-47224 and https://connect.microsoft.com/VisualStudio/feedback/details/1549785
//#      define PHP_COMPILER_THREADSAFE_STATICS
#        define PHP_COMPILER_UDL
#        define PHP_COMPILER_UNICODE_STRINGS
// Uniform initialization is not working yet -- build errors with QUuid
//#      define PHP_COMPILER_UNIFORM_INIT
#        define PHP_COMPILER_UNRESTRICTED_UNIONS
#     endif // _MSC_VER >= 1900
#     if _MSC_FULL_VER >= 190023419
#        define PHP_COMPILER_ATTRIBUTES
// Almost working, see https://connect.microsoft.com/VisualStudio/feedback/details/2011648
//#      define PHP_COMPILER_CONSTEXPR
#        define PHP_COMPILER_THREADSAFE_STATICS
#        define PHP_COMPILER_UNIFORM_INIT
#     endif // _MSC_FULL_VER >= 190023419
#  endif // __cplusplus
#endif // defined(PHP_CC_MSVC) && !defined(PHP_CC_INTEL)

#if defined(__cplusplus)
#include <utility>
#  if (defined(PHP_CC_CLANG) || defined(PHP_CC_INTEL)) && defined(__GNUC_LIBSTD__) \
      || ((__GNU_LIBSTD__-0) * 100 + __GNU_LIBSTD_MINOR__-0 <= 402)
// Apple has not updated libstdc++ since 2007, which means it does not have
// <initializer_list> or std::move. Let's disable these features
#     undef PHP_COMPILER_INITIALIZER_LISTS
#     undef PHP_COMPILER_RVALUE_REFS
#     undef PHP_COMPILER_REF_QUALIFIERS
// Also disable <atomic>, since it's clearly not there
#     undef PHP_COMPILER_ATOMICS
#  endif
#  if defined(PHP_CC_CLANG) && defined(PHP_CC_INTEL) && PHP_CC_INTEL >= 1500
// ICC 15.x and 16.0 have their own implementation of std::atomic, which is activated when in Clang mode
// (probably because libc++'s <atomic> on OS X failed to compile), but they're missing some
// critical definitions. (Reported as Intel Issue ID 6000117277)
#     define __USE_CONSTEXPR 1
#     define __USE_NOEXCEPT 1
#  elif defined(PHP_CC_MSVC) && (defined(PHP_CC_CLANG) || defined(PHP_CC_INTEL))
// Clang and the Intel compiler support more C++ features than the Microsoft compiler
// so make sure we don't enable them if the MS headers aren't properly adapted.
#     ifndef _HAS_CONSTEXPR
#        undef PHP_COMPILER_CONSTEXPR
#     endif
#     ifndef _HAS_DECLTYPE
#        undef PHP_COMPILER_DECLTYPE
#     endif
#     ifndef _HAS_INITIALIZER_LISTS
#        undef PHP_COMPILER_INITIALIZER_LISTS
#     endif
#     ifndef _HAS_NULLPTR_T
#        undef PHP_COMPILER_NULLPTR
#     endif
#     ifndef _HAS_RVALUE_REFERENCES
#        undef PHP_COMPILER_RVALUE_REFS
#     endif
#     ifndef _HAS_SCOPED_ENUM
#        undef PHP_COMPILER_CLASS_ENUM
#     endif
#     ifndef _HAS_TEMPLATE_ALIAS
#        undef PHP_COMPILER_TEMPLATE_ALIAS
#     endif
#     ifndef _HAS_VARIADIC_TEMPLATES
#        undef PHP_COMPILER_VARIADIC_TEMPLATES
#     endif
#  elif defined(_LIBCPP_VERSION)
// libc++ uses __has_feature(cxx_atomic), so disable the feature if the compiler
// doesn't support it. That's required for the Intel compiler 14.x or earlier on OS X, for example.
#     if !__has_feature(cxx_atomic)
#        undef PHP_COMPILER_ATOMICS
#     endif
#  endif
#  if defined(PHP_COMPILER_THREADSAFE_STATICS) && defined(PHP_OS_MAC)
// Apple's low-level implementation of the C++ support library
// (libc++abi.dylib, shared between libstdc++ and libc++) has deadlocks. The
// C++11 standard requires the deadlocks to be removed, so this will eventually
// be fixed; for now, let's disable this.
#     undef PHP_COMPILER_THREADSAFE_STATICS
#  endif
#endif // defined(__cplusplus)

/*
 * C++11 keywords and expressions
 */
#ifdef PHP_COMPILER_NULLPTR
#  define PHP_NULLPTR nullptr
#else 
#  define PHP_NULLPTR NULL
#endif

#ifdef PHP_COMPILER_DEFAULT_MEMBERS
#  define PHP_DECL_EQ_DEFAULT = default
#else
#  define PHP_DECL_EQ_DEFAULT 
#endif

#ifdef PHP_COMPILER_DELETE_MEMBERS
#  define PHP_DECL_EQ_DELETE = delete
#else
#  define PHP_DECL_EQ_DELETE 
#endif

// Don't break code that is already using PHP_COMPILER_DEFAULT_DELETE_MEMBERS
#if defined(PHP_COMPILER_DEFAULT_MEMBERS) && defined(PHP_COMPILER_DELETE_MEMBERS)
#  define PHP_COMPILER_DEFAULT_DELETE_MEMBERS
#endif

#if defined(PHP_COMPILER_CONSTEXPR)
#  if defined(__cpp_constexpr) && __cpp_constexpr-0 >= 201304
#     define PHP_DECL_CONSTEXPR constexpr
#     define PHP_DECL_RELAXED_CONSTEXPR constexpr
#     define PHP_CONSTEXPR constexpr
#     define PHP_RELAXED_CONSTEXPR constexpr
#  else
#     define PHP_DECL_CONSTEXPR constexpr
#     define PHP_DECL_RELAXED_CONSTEXPR
#     define PHP_CONSTEXPR constexpr
#     define PHP_RELAXED_CONSTEXPR const
#  endif
#else
#  define PHP_DECL_CONSTEXPR
#  define PHP_DECL_RELAXED_CONSTEXPR
#  define PHP_CONSTEXPR const
#  define PHP_RELAXED_CONSTEXPR const
#endif

#ifdef PHP_COMPILER_EXPLICIT_OVERRIDES
#  define PHP_DECL_OVERRIDE override
#  define PHP_DECL_FINAL final
#else
#  ifndef PHP_DECL_OVERRIDE
#     define PHP_DECL_OVERRIDE
#  endif
#  ifndef PHP_DECL_FINAL
#     define PHP_DECL_FINAL
#  endif
#endif

#ifdef PHP_COMPILER_NOEXCEPT
#  define PHP_DECL_NOEXCEPT noexcept
#  define PHP_DECL_NOEXCEPT_EXPR(x) noexcept(x)
#  ifdef PHP_DECL_NOTHROW
#     undef PHP_DECL_NOTHROW // override with C++11 noexcept if available
#  endif
#else
#  define PHP_DECL_NOEXCEPT
#  define PHP_DECL_NOEXCEPT_EXPR(x)
#endif

#ifndef PHP_DECL_NOTHROW
#  define PHP_DECL_NOTHROW PHP_DECL_NOEXCEPT
#endif

#if defined(PHP_COMPILER_ALIGNOF)
#  undef PHP_ALIGNOF
#  define PHP_ALIGNOF(x) alignof(x)
#endif

#if defined(PHP_COMPILER_ALIGNAS)
#  undef PHP_DECL_ALIGN
#  undef PHP_DECL_ALIGN(n) alignas(n)
#endif

/*
 * Fallback macros to certain compiler features
 */
#ifndef PHP_NORETURN
#  define PHP_NORETURN
#endif

#ifndef PHP_LIKELY
#  define PHP_LIKELY(x) (x)
#endif

#ifndef PHP_UNLIKELY
#  define PHP_UNLIKELY(x) (x)
#endif

#ifndef PHP_ASSUME_IMPL
#  define PHP_ASSUME_IMPL(expr)  php_noop()
#endif

#ifndef PHP_UNREACHABLE_IMPL
#  define PHP_UNREACHABLE_IMPL() php_noop()
#endif

#ifndef PHP_ALLOC_SIZE
#  define PHP_ALLOC_SIZE(x)
#endif

#ifndef PHP_REQUIRED_RESULT
#  define PHP_REQUIRED_RESULT
#endif

#ifndef PHP_DECL_DEPRECATED
#  define PHP_DECL_DEPRECATED
#endif

#ifndef PHP_DECL_VARIABLE_DEPRECATED
#  define PHP_DECL_VARIABLE_DEPRECATED PHP_DECL_DEPRECATED
#endif

#ifndef PHP_DECL_DEPRECATED_X
#  define PHP_DECL_DEPRECATED_X(text) PHP_DECL_DEPRECATED
#endif

#ifndef PHP_DECL_EXPORT
#  define PHP_DECL_EXPORT
#endif

#ifndef PHP_DECL_IMPORT
#  define PHP_DECL_IMPORT
#endif

#ifndef PHP_DECL_HIDDEN
#  define PHP_DECL_HIDDEN
#endif

#ifndef PHP_DECL_UNUSED
#  define PHP_DECL_UNUSED
#endif

#ifndef PHP_DECL_UNUSED_MEMBER
#  define PHP_DECL_UNUSED_MEMBER
#endif

#ifndef PHP_FUNC_INFO
#  if defined(PHP_OS_SOLARIS) || defined(PHP_CC_XLC)
#     define PHP_FUNC_INFO __FILE__ "(line number unavailable)"
#  else
#     define PHP_FUNC_INFO __FILE__ ":" PHP_STRINGIFY(__LINE__)
#  endif
#endif

#ifndef PHP_DECL_CF_RETURNS_RETAINED
#  define PHP_DECL_CF_RETURNS_RETAINED
#endif

#ifndef PHP_DECL_NS_RETURNS_AUTORELEASED
#  define PHP_DECL_NS_RETURNS_AUTORELEASED
#endif

#ifndef PHP_DECL_PURE_FUNCTION
#  define PHP_DECL_PURE_FUNCTION
#endif

#ifndef PHP_DECL_CONST_FUNCTION
#  define PHP_DECL_CONST_FUNCTION PHP_DECL_PURE_FUNCTION
#endif

/*
 * SG10's SD-6 feature detection and some useful extensions from Clang and GCC
 * https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
 * http://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros
 */
#ifdef __has_builtin
#  define PHP_HAS_BUILTIN(x) __has_builtin(x)
#else
#  define PHP_HAS_BUILTIN 0
#endif

#ifdef __has_attribute
#  define PHP_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#  define PHP_HAS_ATTRIBUTE(x) 0
#endif

#ifdef __has_cpp_attribute
#  define PHP_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#  define PHP_HAS_CPP_ATTRIBUTE(x) 0
#endif

#ifdef __has_include
#  define PHP_HAS_INCLUDE(x) __has_include(x)
#else
#  define PHP_HAS_INCLUDE(x) 0
#endif

#ifdef __has_include_next
#  define PHP_HAS_INCLUDE_NEXT(x) __has_include_next(x)
#else
#  define PHP_HAS_INCLUDE_NEXT(x) 0
#endif

/*
 * Warning/diagnostic handling
 */
#define PHP_DO_PRAGMA(text) _Pragma(#text)
#if defined(PHP_CC_INTEL) && defined(PHP_CC_MSVC)
#  undef PHP_DO_PRAGMA
#  define PHP_WARNING_PUSH                    __pragma(warning(push))
#  define PHP_WARNING_POP                     __pragma(warning(pop))
#  define PHP_WARNING_DISABLE_MSVC(number) 
#  define PHP_WARNING_DISABLE_INTEL(number)   __pragma(warning(disable: number))
#  define PHP_WARNING_DISABLE_CLANG(text)
#  define PHP_WARNING_DISABLE_GCC(text)
#elif defined(PHP_CC_INTEL)
// icc: Intel compiler on Linux or OS X
#  define PHP_WARNING_PUSH                    PHP_DO_PRAGMA(warning(push))
#  define PHP_WRANING_POP                     PHP_DO_PRAGMA(warning(pop))
#  define PHP_WARNING_DISABLE_INTEL(number)   PHP_DO_PRAGMA(disable: number)
#  define PHP_WARNING_DISABLE_MSVC(number) 
#  define PHP_WARNING_DISABLE_CLANG(text)
#  define PHP_WARNING_DISABLE_GCC(text)
#elif defined(PHP_CC_MSCV) && _MSC_VER >= 1500 && !defined(PHP_CC_CLANG)
#  undef PHP_DO_PRAGMA
#  define PHP_WARNING_PUSH                    __pragma(warning(push))
#  define PHP_WARNING_POP                     __pragma(warning(pop))
#  define PHP_WARNING_DISABLE_MSVC(number)    __pragma(warning(disable: number))
#  define PHP_WARNING_DISABLE_INTEL(number)   
#  define PHP_WARNING_DISABLE_CLANG(text)
#  define PHP_WARNING_DISABLE_GCC(text)
#elif defined(PHP_CC_CLANG)
#  define PHP_WARNING_PUSH                    PHP_DO_PRAGMA(clang diagnostic push)
#  define PHP_WRANING_POP                     PHP_DO_PRAGMA(clang diagnostic pop)
#  define PHP_WARNING_DISABLE_CLANG(text)     PHP_DO_PRAGMA(clang diagnostic ignored text)
#  define PHP_WARNING_DISABLE_GCC(text)
#  define PHP_WARNING_DISABLE_INTEL(number)   
#  define PHP_WARNING_DISABLE_MSVC(number) 
#elif defined(PHP_CC_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define PHP_WARNING_PUSH                    PHP_DO_PRAGMA(GCC diagnostic push)
#  define PHP_WRANING_POP                     PHP_DO_PRAGMA(GCC diagnostic pop)
#  define PHP_WARNING_DISABLE_GCC(text)       PHP_DO_PRAGMA(GCC diagnostic ignored text)
#  define PHP_WARNING_DISABLE_CLANG(text)
#  define PHP_WARNING_DISABLE_INTEL(number)   
#  define PHP_WARNING_DISABLE_MSVC(number) 
#else
// All other compilers, GCC < 4.6 and MSVC < 2008
#  define PHP_WARNING_PUSH
#  define PHP_WARNING_POP
#  define PHP_WARNING_DISABLE_INTEL(number)
#  define PHP_WARNING_DISABLE_MSVC(number)
#  define PHP_WARNING_DISABLE_CLANG(text)
#  define PHP_WARNING_DISABLE_GCC(text)
#endif // defined(PHP_CC_INTEL) && defined(PHP_CC_MSVC)

/*
   Proper for-scoping in MIPSpro CC
*/
#ifndef PHP_NO_KEYWORDS
#  if defined(PHP_CC_MIPS) || (defined(PHP_CC_HPACC) && defined(__ia64))
#     define for if(0) {} else for
#  endif
#endif

#ifdef PHP_COMPILER_RVALUE_REFS
#  define php_move(x) std::move(x)
#else
#  define php_move(x) (x)
#endif

#define PHP_UNREACHABLE() \
    do {\
        PHP_ASSERT_X(false, "PHP_UNREACHABLE()", "PHP_UNREACHABLE was reached");\
        PHP_UNREACHABLE_IMPL();\
    } while (0)

#define PHP_ASSUME(Expr) \
    do {\
        const bool valueOfExpression = Expr;\
        PHP_ASSERT_X(valueOfExpression, "PHP_ASSUME()", "Assumption in PHP_ASSUME(\"" #Expr "\") was not correct");\
        PHP_ASSUME_IMPL(valueOfExpression);\
    } while (0)

#if PHP_HAS_CPP_ATTRIBUTE(fallthrough)
#  define PHP_FALLTHROUGH() [[fallthrough]]
#elif defined(__cplusplus)
/* Clang can not parse namespaced attributes in C mode, but defines __has_cpp_attribute */
#  if PHP_HAS_CPP_ATTRIBUTE(clang::fallthrough)
#    define PHP_FALLTHROUGH() [[clang::fallthrough]]
#  endif
#endif
#ifndef PHP_FALLTHROUGH
#  define PHP_FALLTHROUGH() (void)0
#endif
/*
    Sanitize compiler feature availability
*/
#if !defined(PHP_PROCESSOR_X86)
#  undef PHP_COMPILER_SUPPORTS_SSE2
#  undef PHP_COMPILER_SUPPORTS_SSE3
#  undef PHP_COMPILER_SUPPORTS_SSSE3
#  undef PHP_COMPILER_SUPPORTS_SSE4_1
#  undef PHP_COMPILER_SUPPORTS_SSE4_2
#  undef PHP_COMPILER_SUPPORTS_AVX
#  undef PHP_COMPILER_SUPPORTS_AVX2
#endif
#if !defined(PHP_PROCESSOR_ARM)
#  undef PHP_COMPILER_SUPPORTS_NEON
#endif
#if !defined(PHP_PROCESSOR_MIPS)
#  undef PHP_COMPILER_SUPPORTS_MIPS_DSP
#  undef PHP_COMPILER_SUPPORTS_MIPS_DSPR2
#endif

#endif // PHP_KERNEL_BASE_COMPILER_DETECTION_H
