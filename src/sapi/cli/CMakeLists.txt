#命令行服务器接口
project(php)
include_directories(${PHP_KERNEL_LIB_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})
set(PHP_SRCS main.cpp)
add_executable(php ${PHP_SRCS})