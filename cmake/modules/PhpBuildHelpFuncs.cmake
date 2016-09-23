# 这个文件定义一些跟编译相关的帮助函数
# 通用函数定义
function(php_update_compile_flags name)
   
endfunction()

function(php_add_symbol_exports target_name export_file)
   
endfunction()

function(php_add_link_opts target_name)
   
endfunction()

function(php_set_output_directory target)
   
endfunction()

function(php_add_library name)
   
endfunction()

# 增加一个单元测试
# test_suite 测试集合名称，通常是一个大的模块放在一起然后给一个名字
# test_name 测试集合里面的单个测试名字
function(add_unittest test_suite test_name)
   if(NOT PHP_OPT_ENABLE_TEST)
      set(EXCLUDE_FROM_ALL ON)
   endif()
   include_directories(${LIBGTEST_INCLUDE_DIR})
   if (NOT PHP_OPT_WITH_THREAD_SUPPORT)
      list(APPEND PHP_COMPILE_DEFINITIONS GTEST_HAS_PTHREAD=0)
   elseif()
      list(APPEND PHP_COMPILE_DEFINITIONS GTEST_HAS_PTHREAD=1)
   endif ()
   set(PHP_REQUIRES_RTTI OFF)
   PHP_M_ADD_EXECUTABLE(${test_name} IGNORE_EXTERNALIZE_DEBUGINFO ${ARGN})
   set(outdir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
   php_set_output_directory(${test_name} BINARY_DIR ${outdir} LIBRARY_DIR ${outdir})
   # libpthreads overrides some standard library symbols, so main
   # executable must be linked with it in order to provide consistent
   # API for all shared libaries loaded by this executable.
   target_link_libraries(${test_name} ${LIBGTEST_LIBRARIES} ${LIBGTEST_MAIN_LIBRARIES} ${PHP_PTHREAD_LIB})
   add_dependencies(${test_suite} ${test_name})
   get_target_property(test_suite_folder ${test_suite} FOLDER)
   if (NOT ${test_suite_folder} STREQUAL "NOTFOUND")
      set_property(TARGET ${test_name} PROPERTY FOLDER "${test_suite_folder}")
   endif ()
endfunction()

function(php_export_executable_symbols target)
   
endfunction()

function(php_externalize_debuginfo name)
   
endfunction()

#相关宏定义
macro(PHP_M_ADD_LIBRARY name)
   
endmacro()

macro(PHP_M_ADD_LOADABLE_MODULE)
   
endmacro()

macro(PHP_M_ADD_EXECUTABLE name)
   
endmacro()

# cmake系统中没有一种方法获取当前已经添加的编译器定义，这个宏我们自己实现相关的功能
# 参数是跟add_definitions一致
macro(PHP_M_ADD_DEFINITIONS)
   foreach(arg ${ARGN})
      if(DEFINED PHP_DEFINITIONS)
         set(PHP_DEFINITIONS "${PHP_DEFINITIONS} ${arg}")
      elseif()
         set(PHP_DEFINITIONS ${arg})
      endif(DEFINED PHP_DEFINITIONS)
   endforeach()
   add_definitions(${ARGN})
endmacro(PHP_M_ADD_DEFINITIONS)

macro(PHP_M_ADD_TARGET target_name)
   
endmacro()

macro(PHP_M_ADD_SUBDIRECTORY project type name)
   
endmacro()