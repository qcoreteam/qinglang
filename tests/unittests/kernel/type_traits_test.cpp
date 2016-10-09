#include "gtest/gtest.h"

#include "php/kernel/base/type_traits.h"

namespace
{
//相关的类型定义
struct Base
{
   int getValue(){
      return 0;
   }
   char *getName()
   {
      return "name";
   }
   
   int setValue(int value)
   {
      return 1;
   }
   
   Base& setName(char &name)
   {
      return *this;
   }
};

struct Derived : Base
{};

struct Other
{};

char name = 'c';

PHP_METHOD_CHECKER(HasGetValue, getValue, int, ());
PHP_METHOD_CHECKER(HasGetName, getName, char *, ());
PHP_METHOD_CHECKER(HasSetValue, setValue, int, (1));
PHP_METHOD_CHECKER(HasSetName, setName, Base&, (name));

TEST(TypeTraitsTests, ClsMethodCheck)
{
   EXPECT_TRUE(HasGetValue<Base>());
   EXPECT_TRUE(HasGetValue<Derived>());
   EXPECT_FALSE(HasGetValue<Other>());
   
   EXPECT_TRUE(HasGetName<Base>());
   EXPECT_TRUE(HasGetName<Derived>());
   EXPECT_FALSE(HasGetName<Other>());
   
   EXPECT_TRUE(HasSetName<Base>());
   EXPECT_TRUE(HasSetName<Derived>());
   EXPECT_FALSE(HasSetName<Other>());
}

}