#include <iostream>
#include "php/kernel/base/global.h"
#include <atomic>
int main(int argc, char* argv[])
{
#     if !__has_feature(cxx_override_control)
   std::cout << PHP_VERSION_STR;
   std::cout << "hello word" << std::endl;
#  error "xiuxiu"
#endif
}