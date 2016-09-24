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
include(PhpBuildHelpFuncs)
include(PhpHandleCommonOpts)

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

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
   message(FATAL_ERROR "不允许In-source编译")
endif()

# 设置相关选项
option(PHP_OPT_ENABLE_VERBOSE_DEBUG  "是否开启详细信息调试编译" OFF)
option(PHP_OPT_ENABLE_TEST           "是否开启测试编译" ON)
option(PHP_OPT_WITH_THREAD_SUPPORT   "是否开启线程支持" ON)
option(PHP_OPT_INCLUDE_DOCS          "是否安装程序文档" ON)
option(PHP_OPT_INSTALL_SUPPORT_FILES "是否安装支持文件" ON)
option(PHP_BUILD_SHARED_LIBS         "把php所有的库都编译成动态链接库" ON)
option(PHP_ENABLE_PIC                "编译Position-Independent代码" ON)
option(PHP_ENABLE_WARNINGS           "打开编译器警告" ON)
option(PHP_ENABLE_MODULES            "使用 C++ modules进行编译" OFF)
option(PHP_ENABLE_CXX1Y              "使用C++1y标准进行编译" ON)
option(PHP_ENABLE_PEDANTIC           "编译的时候输出详细的信息" ON)
option(PHP_ENABLE_WERROR             "当有警告产生的时候就停止编译，这个选项对于消除编译警告很有作用" OFF)

if(PHP_TEMP_UPPERCASE_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
   option(PHP_ENABLE_ASSERTIONS "打开assertions宏输出" ON)
elseif()
   option(PHP_ENABLE_ASSERTIONS "关闭assertions宏输出" OFF)
endif()

# 处理相关选项
if(PHP_OPT_ENABLE_TEST)
   find_package(GoogleTest)
endif()

# 全局的相关变量设置
set(PHP_KERNEL_LIB_INCLUDES)
set(PHP_LANG_INCLUDES)
set(ZEND_ENGINE_INCLUDES)
set(PHP_LIBDIR_SUFFIX "" CACHE STRING "定义32/64位系统的库文件目录的后缀")
set(PHP_TOOLS_INSTALL_DIR "bin" CACHE STRING "设置系统工具二进制文件的安装文件夹 ( 默认为:bin ) ")
mark_as_advanced(PHP_TOOLS_INSTALL_DIR)
# 为了让生成的二进制文件便于寻找，我们在这里进行设置统一的输出文件目录
set(PHP_RUNTIME_OUTPUT_INTDIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin)
set(PHP_LIBRARY_OUTPUT_INTDIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib${PHP_LIBDIR_SUFFIX})

if(WIN32 OR CYGWIN)
   # DLL 平台，将dll相关的放进runtime目录
   set(PHP_SHLIB_OUTPUT_INTDIR ${PHP_RUNTIME_OUTPUT_INTDIR})
elseif()
   set(PHP_SHLIB_OUTPUT_INTDIR ${PHP_LIBRARY_OUTPUT_INTDIR})
endif()

set(PHP_TOOLS_BINARY_DIR ${PHP_RUNTIME_OUTPUT_INTDIR}) # --bindir
set(PHP_LIBRARY_DIR      ${PHP_LIBRARY_OUTPUT_INTDIR}) # --libdir
set(PHP_MAIN_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR})  # --src-root
set(PHP_MAIN_INCLUDE_DIR ${PHP_MAIN_SRC_DIR}/include)  # --includedir
set(PHP_BINARY_DIR       ${CMAKE_CURRENT_BINARY_DIR})   # --prefix

set(PHP_BINARY_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)

# 这个变量主要保存当前系统已经添加的编译器标志定义
set(PHP_DEFINITIONS)
set(PHP_COMPILE_DEFINITIONS)
# 判断是否需要加上运行时类型信息
set(PHP_REQUIRES_RTTI ON)
set(PHP_PTHREAD_LIB)
set(PHP_OPTIONAL_SOURCES)
# 内部使用的一些全局变量
# 这个变量主要是用在 PHP_M_ADD_EXECUTABLE 中，如果没有传人参数，那么宏就会寻找全局 PHP_I_EXCLUDE_FROM_ALL 变量进行操作
set(PHP_I_EXCLUDE_FROM_ALL)

# 设置一些全局的属性，便于操作
set_property(GLOBAL PROPERTY PHP_I_DISPLAY_FILES)

# 探测系统包依赖
include(PhpDetectRequirePackages)
include(PhpCheckRequireHeaders)


# 探测系统编译环境

# 配置相关系统
configure_file("${CMAKE_SOURCE_DIR}/php_prefix.h.cmake" "${CMAKE_BINARY_DIR}/include/php_prefix.h")
configure_file("${CMAKE_SOURCE_DIR}/php_compiler.h.cmake" "${CMAKE_BINARY_DIR}/include/php_compiler.h")
configure_file("${CMAKE_SOURCE_DIR}/php_config.h.cmake" "${CMAKE_BINARY_DIR}/include/php_config.h")

# 设置标准的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PHP_BINARY_DIR}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PHP_BINARY_DIR}/lib${PHP_LIBDIR_SUFFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PHP_BINARY_DIR}/lib${PHP_LIBDIR_SUFFIX})

# 设置cmake系统变量
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

if (APPLE)
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
else(UNIX)
  if(NOT DEFINED CMAKE_INSTALL_RPATH)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib${PHP_LIBDIR_SUFFIX}")
    if(${CMAKE_SYSTEM_NAME} MATCHES "(FreeBSD|DragonFly)")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,origin")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,origin")
    endif()
  endif(NOT DEFINED CMAKE_INSTALL_RPATH)
endif()

if (UNIX AND NOT APPLE AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "SunOS|AIX")
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -Wl,-allow-shlib-undefined")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 全局的编译器相关的设置
include_directories(${PHP_MAIN_INCLUDE_DIR} ${PHP_BINARY_INCLUDE_DIR})

if(MINGW)
  # People report that -O3 is unreliable on MinGW. The traditional
  # build also uses -O2 for that reason:
  php_replace_compiler_option(CMAKE_CXX_FLAGS_RELEASE "-O3" "-O2")
endif()

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
php_add_file_to_display_pool(
   LICENSE.txt 
   NEWS.txt
   README.txt
   INSTALL.txt
   NOTES.txt)
php_add_display_target()

# 安装相关文件
include(extra_files_installer.cmake)
