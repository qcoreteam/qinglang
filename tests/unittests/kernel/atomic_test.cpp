#ifdef PHP_ATOMIC_FORCE_CXX11
// We need to check if this compiler has C++11 atomics and constexpr support.
// We can't rely on qcompilerdetection.h because it forces all of qglobal.h to
// be included, which causes qbasicatomic.h to be included too.
// Incomplete, but ok
#  if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1500 && (__cplusplus >= 201103L || defined(__INTEL_CXX11_MODE__))
#  elif defined(__clang__) && (__cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__))
#    if !__has_feature(cxx_constexpr) || !__has_feature(cxx_atomic) || !__has_include(<atomic>)
#      undef PHP_ATOMIC_FORCE_CXX11
#    endif
#  elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && (__cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__))
#  elif defined(_MSC_VER) && _MSC_VER >= 1900
    // We need MSVC 2015 because of: atomics (2012), constexpr (2015), and unrestricted unions (2015).
    // Support for constexpr is not working completely on MSVC 2015 but it's enough for the test.
#  else
#    undef PHP_ATOMIC_FORCE_CXX11
#  endif
#  ifndef PHP_ATOMIC_FORCE_CXX11
#    undef PHP_ATOMIC_TEST_TYPE
#    define PHP_ATOMIC_TEST_TYPE unsupported
#  endif
#endif

#include "gtest/gtest.h"
#include "php/kernel/thread/atomic.h"

namespace 
{

#define TYPE_SUPPORTED_int                1
#define TYPE_SUPPORTED_uint               1
#define TYPE_SUPPORTED_long               1
#define TYPE_SUPPORTED_ulong              1
#define TYPE_SUPPORTED_ptrdiff_t          1
#define TYPE_SUPPORTED_uintptr_t          1

#if (defined(__SIZEOF_WCHAR_T__) && (__SIZEOF_WCHAR_T__-0) > 2) \
    || (defined(WCHAR_MAX) && (WCHAR_MAX-0 > 0x10000))
#  define TYPE_SUPOORTED_wchat_t          1
#endif
#ifdef PHP_COMPILER_UNICODE_STRINGS
#  define TYPE_SUPPORTED_char32_t         1
#endif

#ifdef PHP_ATOMIC_INT8_IS_SUPPORTED
#  define TYPE_SUPPORTED_char             1
#  define TYPE_SUPPORTED_uchar            1
#  define TYPE_SUPPORTED_schar            1
#endif

#ifdef PHP_ATOMIC_INT16_IS_SUPPORTED
#  define TYPE_SUPPORTED_short            1
#  define TYPE_SUPPORTED_ushort           1
#  ifdef PHP_COMPILER_UNICODE_STRINGS     
#     define TYPE_SUPOORTED_char16_t      1
#  endif
#  ifndef TYPE_SUPOORTED_wchat_t
#     define TYPE_SUPOORTED_wchat_t       1
#  endif
#endif

#ifdef PHP_ATOMIC_INT64_IS_SUPPORTED
#  define TYPE_SUPPORTED_php_longlong     1
#  define TYPE_SUPPORTED_php_ulonglong    1
#endif

#define PHP_ATOMIC_TYPE_SUPPORTED2(type)    TYPE_SUPPORTED_ ## type
#define PHP_ATOMIC_TYPE_SUPPORTED(type)     PHP_ATOMIC_TYPE_SUPPORTED2(type)

#if PHP_ATOMIC_TYPE_SUPPORTED(PHP_ATOMIC_TEST_TYPE)
#  define TEST_TYPE PHP_ATOMIC_TEST_TYPE
#else
#  define TEST_TYPE int
#  define PHP_ATOMIC_TEST_NOT_SUPPORTED
#endif

using schar = signed char;
using Type = TEST_TYPE;
using T = Type;
using Php::Kernel::Thread::AtomicInteger;
enum {
    TypeIsUnsigned = Type(-1) > Type(0),
    TypeIsSigned = !TypeIsUnsigned
};

template <bool> 
struct LargeIntTemplate;

template <>     
struct LargeIntTemplate<true>  
{ 
   using Type = Php::php_uint64;
};

template <>     
struct LargeIntTemplate<false> 
{ 
   using Type = Php::php_int64;
};

using LargeInt = LargeIntTemplate<TypeIsUnsigned>::Type;

template<typename T>
class AtomicTest : public ::testing::Test
{
   using AtomictType = T;
};

using AtomicTestTypes = ::testing::Types<int>;
TYPED_TEST_CASE(AtomicTest, AtomicTestTypes);

TYPED_TEST(AtomicTest, MacroTest)
{
#if !defined(PHP_ATOMIC_INT32_IS_SUPPORTED)
   bool atomic32Supported = false;
#else
   bool atomic32Supported = true;
#endif
   EXPECT_TRUE(atomic32Supported) << "AtomicInteger必须支持32位类型";
#if PHP_POINTER_SIZE == 8 && !defined(PHP_ATOMIC_INT64_IS_SUPPORTED)
   bool atomic64SupportCondMatch = false;
#else
   bool atomic64SupportCondMatch = true;
#endif
   EXPECT_TRUE(atomic32Supported) << "AtomicInteger64位支持条件错误";
}

TYPED_TEST(AtomicTest, StaticChecks)
{
   EXPECT_TRUE(sizeof(AtomicInteger<T>) == sizeof(T));
   EXPECT_TRUE(PHP_ALIGNOF(AtomicInteger<T>) == PHP_ALIGNOF(T));

}

}