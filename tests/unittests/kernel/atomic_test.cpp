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

#include <vector>

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

#define DEF_ATOMIC_TYPE_CHECKER(type) \
   template <> \
   struct AtomicTypeChecker<type> : public std::true_type \
{}

template <typename T>
struct AtomicTypeChecker : public std::false_type
{};

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
      
using uchar = unsigned char;
using schar = signed char;

using php_uintptr_t = Php::php_uintptr_t;
using php_intptr_t = Php::php_intptr_t;
using php_longlong = Php::php_longlong;
using php_ulonglong = Php::php_ulonglong;

DEF_ATOMIC_TYPE_CHECKER(int);
DEF_ATOMIC_TYPE_CHECKER(uint);
DEF_ATOMIC_TYPE_CHECKER(long);
DEF_ATOMIC_TYPE_CHECKER(ulong);
DEF_ATOMIC_TYPE_CHECKER(php_intptr_t);
DEF_ATOMIC_TYPE_CHECKER(php_uintptr_t);
DEF_ATOMIC_TYPE_CHECKER(wchar_t);
DEF_ATOMIC_TYPE_CHECKER(char32_t);
DEF_ATOMIC_TYPE_CHECKER(char);
DEF_ATOMIC_TYPE_CHECKER(uchar);
DEF_ATOMIC_TYPE_CHECKER(schar);
DEF_ATOMIC_TYPE_CHECKER(short);
DEF_ATOMIC_TYPE_CHECKER(ushort);
DEF_ATOMIC_TYPE_CHECKER(char16_t);

//DEF_ATOMIC_TYPE_CHECKER(php_longlong);
//DEF_ATOMIC_TYPE_CHECKER(php_ulonglong);

using Php::Kernel::Thread::AtomicInteger;

template<typename T>
class AtomicTests : public ::testing::Test
{
public:
   using AtomictType = T;
   using LargeInt = typename LargeIntTemplate<std::is_unsigned<T>::value>::Type;
   
public:
   static void SetUpTestCase()
   {
      
   }
protected:
//   std::vector<LargeInt> m_
};

using AtomicTestTypes = ::testing::Types<int, uint, long, ulong,
php_intptr_t, php_uintptr_t,wchar_t,
char32_t, char, uchar, schar, short, ushort,
char16_t,x>;

TYPED_TEST_CASE(AtomicTests, AtomicTestTypes);

TYPED_TEST(AtomicTests, MacroTest)
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

PHP_METHOD_CHECKER(IsReferenceCountingNativeChecker, isReferenceCountingNative, bool, ());
PHP_METHOD_CHECKER(IsReferenceCountingWaitFreeChecker, isReferenceCountingWaitFree, bool, ());
PHP_METHOD_CHECKER(IsTestAndSetNativeChecker, isTestAndSetNative, bool, ());
PHP_METHOD_CHECKER(IsTestAndSetWaitFreeChecker, isTestAndSetWaitFree, bool, ());
PHP_METHOD_CHECKER(IsFetchAndStoreNativeChecker, isFetchAndStoreNative, bool, ());
PHP_METHOD_CHECKER(IsFetchAndStoreWaitFreeChecker, isFetchAndStoreWaitFree, bool, ());
PHP_METHOD_CHECKER(IsFetchAndAddNativeChecker, isFetchAndAddNative, bool, ());
PHP_METHOD_CHECKER(IsFetchAndAddWaitFreeChecker, isFetchAndAddWaitFree, bool, ());

TYPED_TEST(AtomicTests, StaticChecks)
{
   using AtomictType = typename TestFixture::AtomictType;
   ASSERT_TRUE(AtomicTypeChecker<AtomictType>()) << "当前平台不支持当前的Atomic类型";
   EXPECT_TRUE(sizeof(AtomicInteger<AtomictType>) == sizeof(AtomictType));
   EXPECT_TRUE(PHP_ALIGNOF(AtomicInteger<AtomictType>) == PHP_ALIGNOF(AtomictType));
   EXPECT_TRUE(IsReferenceCountingNativeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsReferenceCountingWaitFreeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsTestAndSetNativeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsTestAndSetWaitFreeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsFetchAndStoreNativeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsFetchAndStoreWaitFreeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsFetchAndAddNativeChecker<AtomicInteger<AtomictType>>());
   EXPECT_TRUE(IsFetchAndAddWaitFreeChecker<AtomicInteger<AtomictType>>());
}

}