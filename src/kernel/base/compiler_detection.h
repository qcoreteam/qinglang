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

#endif // PHP_KERNEL_BASE_COMPILER_DETECTION_H
