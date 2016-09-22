cmake_minimum_required(VERSION 3.6)
project(phpplusplus)
#设置modules的路径
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
    ${CMAKE_MODULE_PATH})

project(php_plusplus CXX)

#设置相关版本信息
set(PHP_VERSION_MAJOR 0)
set(PHP_VERSION_MINOR 0)
set(PHP_VERSION_PATCH 1)
set(PHP_VERSION_STAGE "alpha")
set(PHP_PACKAGE_NAME "php++ engine")
set(PHP_VERSION_STR "${PHP_VERSION_MAJOR}.${PHP_VERSION_MINOR}.${PHP_VERSION_PATCH}_${PHP_VERSION_STAGE}")
set(PHP_PACKAGE_VERSION "${PHP_PACKAGE_NAME} ${PHP_VERSION_STR}")

#加载相关依赖库
include(PhpCompilerFlagFuncs)
include(PhpUtilFuncs)

#设置相关选项

#处理相关选项

#配置相关系统

#全局的编译器相关的设置
include_directories(include)
#处理相关子文件夹文件
add_subdirectory(cmake/modules)
add_subdirectory(include)
add_subdirectory(assets)
add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)