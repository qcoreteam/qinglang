# 这个文件定义一些跟编译相关的帮助函数
# 通用函数定义
function(php_update_compile_flags name)
   
endfunction()

function(php_add_symbol_exports target_name export_file)
   
endfunction()

function(php_add_link_opts target_name)
   
endfunction()

# 根据${CMAKE_CONFIGURATION_TYPES}的值设置target的输出文件夹.
# 提示：不要哦直接的设置CMAKE_*_OUTPUT_DIRECTORY等cmake变量了，或者一个特定的builder，例如msbuild.exe，可能会混淆
function(php_set_output_directory target)
   cmake_parse_arguments(PHP_ARG "BINARY_DIR;LIBRARY_DIR" "" ${ARGN})
   # module_dir -- 对应LIBRARY_OUTPUT_DIRECTORY.
   # 他会影响add_library(MODULE)的输出.
   if(WIN32 OR CYGWIN)
      # DLL平台
      set(module_dir ${PHP_ARG_BINARY_DIR})
   elseif()
      set(module_dir ${PHP_ARG_LIBRARY_DIR})
   endif()
   if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      foreach(build_mode ${CMAKE_CONFIGURATION_TYPES})
         string(TOUPPER "${build_mode}" config_suffix)
         if(PHP_ARG_BINARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} bi ${PHP_ARG_BINARY_DIR})
            set_target_properties(${target} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_${config_suffix}" ${bi})
         endif()
         if(PHP_ARG_LIBRARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} li ${PHP_ARG_LIBRARY_DIR})
            set_target_properties(${target} PROPERTIES "ARCHIVE_OUTPUT_DIRECTORY_${config_suffix}" ${li})
         endif()
         if(module_dir)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} mi ${module_dir})
            set_target_properties(${target} PROPERTIES "LIBRARY_OUTPUT_DIRECTORY_${config_suffix}" ${mi})
         endif()
      endforeach()
   elseif()
      if(PHP_ARG_BINARY_DIR)
         set_target_properties(${target} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY" ${PHP_ARG_BINARY_DIR})
      endif()
      if(PHP_ARG_LIBRARY_DIR)
         set_target_properties(${target} PROPERTIES "ARCHIVE_OUTPUT_DIRECTORY" ${PHP_ARG_LIBRARY_DIR})
      endif()
      if(module_dir)
         set_target_properties(${target} PROPERTIES "LIBRARY_OUTPUT_DIRECTORY" ${module_dir})
      endif()
   endif()
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