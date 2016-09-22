# FindGoogleTest
# ----------------
# 寻找google test框架
#
#   LIBGTEST_FOUND - 系统安装了 GTest
#   LIBGTEST_INCLUDE_DIR - Google Test include directory
#   LIBGTEST_LIBRARIES - Link these to libgtest
#   LIBGTEST_MAIN_LIBRARIES - Link these to libgtest_main
#   LIBGTEST_VERSION_STRING - version of libgtest found (since CMake 2.8.8)
#
set(LIBGTEST_FOUND "ON")
find_path(LIBGTEST_INCLUDE_DIR NAMES gtest/gtest.h
    HINTS
    /usr/local/include
    DOC
    "the include directory of google test library")

find_library(LIBGTEST_LIBRARIES NAMES gtest libgtest
    HINTS
    /usr/local/lib
    DOC
    "the library name of google test library")
find_library(LIBGTEST_MAIN_LIBRARIES NAMES libgtest_main gtest_main
    HINTS
    /usr/local/lib
    DOC
    "the library main name of google test library")
if(LIBGTEST_INCLUDE_DIR AND LIBGTEST_LIBRARIES AND LIBGTEST_MAIN_LIBRARIES)
    set(LIBGTEST_VERSION_STRING "1.7.0")
    set(LIBGTEST_FOUND "YES")
endif()
mark_as_advanced(LIBGTEST_VERSION_STRING LIBGTEST_FOUND
    LIBGTEST_INCLUDE_DIR LIBGTEST_LIBRARIES LIBGTEST_MAIN_LIBRARIES)
