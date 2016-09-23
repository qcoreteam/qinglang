cmake_minimum_required(VERSION 3.6)
project(phpplusplus)
# 设置modules的路径
set(CMAKE_MODULE_PATH
   "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
   ${CMAKE_MODULE_PATH})

project(php_plusplus CXX)

# 设置相关版本信息
set(PHP_VERSION_MAJOR 0)
set(PHP_VERSION_MINOR 0)
set(PHP_VERSION_PATCH 1)
set(PHP_VERSION_STAGE "alpha")
set(PHP_PACKAGE_NAME "php++ engine")
set(PHP_VERSION_STR "${PHP_VERSION_MAJOR}.${PHP_VERSION_MINOR}.${PHP_VERSION_PATCH}_${PHP_VERSION_STAGE}")
set(PHP_PACKAGE_VERSION "${PHP_PACKAGE_NAME} ${PHP_VERSION_STR}")
set(PHP_STAGE_NAME ${PHP_VERSION_STAGE} CACHE STRING "设置stage类型,允许的值有 (alpha|beta|rc|release)")

# 支持的stage名称
set(PHP_P_ALLOWED_STAGE_NAMES
   alpha
   beta
   rc
   release)
list(FIND PHP_P_ALLOWED_STAGE_NAMES ${PHP_STAGE_NAME} PHP_P_STAGE_RET)

if(PHP_P_STAGE_RET EQUAL "-1")
   set(PHP_STAGE_NAME alpha)
endif()

# 加载相关依赖库
include(PhpCompilerFlagFuncs)
include(PhpUtilFuncs)

# 设置相关选项
option(PHP_OPT_ENABLE_VERBOSE_DEBUG "是否开启详细信息调试编译" OFF)
option(PHP_OPT_ENABLE_TEST "是否开启测试编译" ON)
option(PHP_OPT_WITH_THREAD_SUPPORT "是否开启线程支持" ON)
option(PHP_OPT_INCLUDE_DOCS "是否安装程序文档" ON)

# 配置信息合法性检查
if(NOT CMAKE_BUILD_TYPE AND CMAKE_CONFIGURATION_TYPES)
   message(STATUS "没有指定编译类型, 默认设置为 Debug")
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "编译类型 (默认 Debug)")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" PHP_TEMP_UPPERCASE_CMAKE_BUILD_TYPE)
if (CMAKE_BUILD_TYPE AND
      NOT PHP_TEMP_UPPERCASE_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL)$")
   message(FATAL_ERROR "CMAKE_BUILD_TYPE值类型错误: ${CMAKE_BUILD_TYPE}")
endif()

# 处理相关选项
if(PHP_OPT_ENABLE_TEST)
   find_package(GoogleTest)
endif()

# 全局的相关变量设置
set(PHP_KERNEL_LIB_INCLUDES)
set(PHP_LANG_INCLUDES)
set(ZEND_ENGINE_INCLUDES)
# 这个变量主要保存当前系统已经添加的编译器标志定义
set(PHP_DEFINITIONS)
set(PHP_COMPILE_DEFINITIONS)
# 判断是否需要加上运行时类型信息
set(PHP_REQUIRES_RTTI ON)
set(PHP_PTHREAD_LIB)

# 探测系统包依赖
include(PhpDetectRequirePackages)
include(PhpCheckRequireHeaders)
include(PhpBuildHelpFuncs)


# 探测系统编译环境

# 配置相关系统
configure_file("${CMAKE_SOURCE_DIR}/php_prefix.h.cmake" "${CMAKE_BINARY_DIR}/include/php_prefix.h")
configure_file("${CMAKE_SOURCE_DIR}/php_compiler.h.cmake" "${CMAKE_BINARY_DIR}/include/php_compiler.h")
configure_file("${CMAKE_SOURCE_DIR}/php_config.h.cmake" "${CMAKE_BINARY_DIR}/include/php_config.h")

# 全局的编译器相关的设置
include_directories(include ${CMAKE_BINARY_DIR}/include)

# 处理相关子文件夹文件
add_subdirectory(cmake/modules)
add_subdirectory(include)
add_subdirectory(assets)
add_subdirectory(src)
add_subdirectory(tools)

if(PHP_OPT_ENABLE_TEST)
   add_subdirectory(tests)
endif()

if(PHP_OPT_INCLUDE_DOCS)
   add_subdirectory(docs)
endif()

# 安装相关文件
include(extra_files_installer.cmake)