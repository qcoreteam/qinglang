cmake_minimum_required(VERSION 3.6)
project(phpplusplus)
#设置modules的路径
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
    ${CMAKE_MODULE_PATH})

project(php_plusplus CXX)

#处理相关子文件夹文件
add_subdirectory(cmake/modules)
add_subdirectory(include)
add_subdirectory(assets)
add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(tools)