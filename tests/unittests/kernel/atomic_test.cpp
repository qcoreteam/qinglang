#include "gtest/gtest.h"
#include "php/kernel/thread/atomic.h"

namespace 
{

#define TYPE_SUPPORTED_int        1
#define TYPE_SUPPORTED_uint       1
#define TYPE_SUPPORTED_long       1
#define TYPE_SUPPORTED_ulong      1
#define TYPE_SUPPORTED_

TEST(AtomicTest, TestMacros){

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

}