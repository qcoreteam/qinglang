#include <iostream>
#include "php/kernel/base/global.h"
int main(int argc, char* argv[])
{
   
#     if __has_feature(cxx_auto_type)
   std::cout << PHP_VERSION_STR;
   std::cout << "hello word" << std::endl;
#endif
}