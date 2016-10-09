set(KERNEL_TEST_SRCS
   global_test.cpp
   atomic_test.cpp
   type_traits_test.cpp)

php_m_add_unittest(KernelTests ${KERNEL_TEST_SRCS})