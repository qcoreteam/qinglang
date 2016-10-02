#include "gtest/gtest.h"
#include "php/kernel/thread/atomic.h"

namespace 
{

TEST(AtomicTest, TestMacros){

#if !defined(PHP_ATOMIC_INT32_IS_SUPPORTED)
   bool atomic32Supported = false;
#else
   bool atomic32Supported = false;
#endif
   EXPECT_TRUE(atomic32Supported) << "AtomicInteger必须支持32位类型";
}

}