#include <iostream>
#include "php/kernel/base/global.h"
int main(int argc, char* argv[])
{
   std::cout << PHP_VERSION_STR;
   std::cout << "hello word" << std::endl;
}