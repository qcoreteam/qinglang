#这个文件定义一些常用的工具类

function(php_install_library_symlink name dest type)
   cmake_parse_arguments(PHP_ARG "ALWAYS_GENERATE" "COMPONENT" "" ${ARGN})
   foreach(path ${CMAKE_MODULE_PATH})
      if(EXISTS ${path}/PhpInstallSymlink.cmake)
         set(INSTALL_SYMLINK ${path}/PhpInstallSymlink.cmake)
         break()
      endif()
   endforeach()
   set(component ${PHP_ARG_COMPONENT})
   if(NOT component)
      set(component ${name})
   endif()
   set(full_name ${CMAKE_${type}_LIBRARY_PREFIX}${name}${CMAKE_${type}_LIBRARY_SUFFIX})
   set(full_dest ${CMAKE_${type}_LIBRARY_PREFIX}${dest}${CMAKE_${type}_LIBRARY_SUFFIX})
   set(output_dir lib${PHP_LIBDIR_SUFFIX})
   if(WIN32 AND "${type}" STREQUAL "SHARED")
      set(output_dir bin)
   endif()
   install(SCRIPT ${INSTALL_SYMLINK}
      CODE "install_symlink(${full_name} ${full_dest} ${output_dir})"
      COMPONENT ${component})
   if (NOT CMAKE_CONFIGURATION_TYPES AND NOT PHP_ARG_ALWAYS_GENERATE)
      add_custom_target(install-${name}
         DEPENDS ${name} ${dest} install-${dest}
         COMMAND "${CMAKE_COMMAND}"
         -DCMAKE_INSTALL_COMPONENT=${name}
         -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
   endif()
endfunction()

function(php_install_symlink name dest)
   cmake_parse_arguments(PHP_ARG "ALWAYS_GENERATE" "" "" ${ARGN})
   foreach(path ${CMAKE_MODULE_PATH})
      if(EXISTS ${path}/PhpInstallSymlink.cmake)
         set(INSTALL_SYMLINK ${path}/PhpInstallSymlink.cmake)
         break()
      endif()
   endforeach()
   if(PHP_ARG_ALWAYS_GENERATE)
      set(component ${dest})
   else()
      set(component ${name})
   endif()
   set(full_name ${name}${CMAKE_EXECUTABLE_SUFFIX})
   set(full_dest ${dest}${CMAKE_EXECUTABLE_SUFFIX})
   install(SCRIPT ${INSTALL_SYMLINK}
      CODE "install_symlink(${full_name} ${full_dest} ${PHP_INSTALL_DIR})"
      COMPONENT ${component})
   if (NOT CMAKE_CONFIGURATION_TYPES AND NOT PHP_ARG_ALWAYS_GENERATE)
      add_custom_target(install-${name}
         DEPENDS ${name} ${dest} install-${dest}
         COMMAND "${CMAKE_COMMAND}"
         -DCMAKE_INSTALL_COMPONENT=${name}
         -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
   endif()
endfunction()

# 替换编译器选项值 
# Example: php_replace_compiler_option(CMAKE_CXX_FLAGS_RELEASE "-O3" "-O2")
# var 变量的名称 
# old 被替换的值
# new 替换的值
function(php_replace_compiler_option var old new)
   set(org_value ${${var}})
   if("${org_value}" MATCHES "(^| )${new}($| )")
      set(target_new_value "")
   elseif()
      set(target_new_value "${new}")
   endif()
   if("${org_value}" MATCHES "(^| )${old}($| )")
      string(REGEX REPLACE "(^| )${old}($| )" ${target_new_value} ${var} "${org_value}")
   elseif()
      # 直接添加, 存在的话 target_new_value 为空
      set(${var} "${${var}} ${target_new_value}")
   endif()
   # ${var}变量是本地，将值设置到全局
   set(${var} ${${var}} PARENT_SCOPE)
endfunction()

macro(php_m_add_td_sources srcs)
   file(GLOB tds *.td)
   if(tds)
      source_group("TableGen descriptions" FILES ${tds})
      set_source_files_properties(${tds} PROPERTIES HEADER_FILE_ONLY ON)
      list(APPEND ${srcs} ${tds})
   endif()
endmacro(php_m_add_td_sources)

# 使用通配符增加头文件
# header_files_out 识别的头文件列表
# glob 识别通配符
function(php_add_header_files_for_glob header_files_out glob)
   file(GLOB header_files ${glob})
   set(${header_files_out} ${header_files} PARENT_SCOPE)
endfunction(php_add_header_files_for_glob)

# 寻找当前目录下的头文件以及指定additional_header_dirs里面的头文件
# headers_out 搜索到的头文件的列表
# additional_header_dirs 附加的头文件目录
function(php_find_all_header_files headers_out additional_header_dirs)
   php_add_header_files_for_glob(headers *.h)
   list(APPEND all_headers ${headers})
   foreach(additional_dir ${additional_header_dirs})
      php_add_header_files_for_glob(headers "${additional_dir}/*.h")
      list(APPEND all_headers ${headers})
      php_add_header_files_for_glob(headers "${additional_dir}/*.inc")
      list(APPEND all_headers ${headers})
   endforeach()
   set(${headers_out} ${all_headers} PARENT_SCOPE)
endfunction(php_find_all_header_files)

# 为了让 MSVC 和 xcode 把所有的项目文件显示出来我们在这里把headers也全部加入到 source code 列表中
# out_var 输出的sources文件集合
# out_var 后面是任意文件列表
function(php_process_sources out_var)
   cmake_parse_arguments(PHP_ARG "" "" "ADDITIONAL_HEADERS;ADDITIONAL_HEADER_DIRS" ${ARGN})
   set(sources ${PHP_ARG_UNPARSED_ARGUMENTS})
   php_check_source_file_list(${sources})
   if(MSVC OR XCODE)
      php_m_add_td_sources(sources)
      php_find_all_header_files(headers "${PHP_ARG_ADDITIONAL_HEADER_DIRS}")
      if(headers)
         set_source_files_properties(${headers} PROPERTIES HEADER_FILE_ONLY ON)
      endif()
      set_source_files_properties(${PHP_ARG_ADDITIONAL_HEADERS} PROPERTIES HEADER_FILE_ONLY ON)
      list(APPEND sources ${PHP_ARG_ADDITIONAL_HEADERS} ${headers})
   endif()
   set(${out_var} ${sources} PARENT_SCOPE)
endfunction()

# 检查当前项目文件夹是否含有不需要的文件
# 参数的skip 文件件列表
function(php_check_source_file_list)
   set(skip_files ${ARGN})
   file(GLOB current_dir_files *.c *.cpp)
   foreach(cur_path ${current_dir_files})
      get_filename_component(filename ${cur_path} NAME)
      # 排除隐藏文件
      if(NOT "${filename}" MATCHES "^\\.")
         list(FIND PHP_OPTIONAL_SOURCES ${filename} idx)
         if(idx LESS 0)
            list(FIND skip_files ${filename} idx)
            if(idx LESS 0)
               message(SEND_ERROR "发现未知源代码 ${cur_path}
                  Please update ${CMAKE_CURRENT_LIST_FILE}\n")
            endif()
         endif()
      endif()
   endforeach()
endfunction(php_check_source_file_list)

# 这个函数主要是解决Qt creator有些文件不显示的问题
function(php_add_file_to_display_pool)
   get_property(display_files GLOBAL PROPERTY PHP_I_DISPLAY_FILES)
   foreach(file_path ${ARGN})
      list(FIND display_files ${file_path} idx)
      if(idx LESS 0)
         list(APPEND display_files "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")
      endif()
   endforeach()
   set_property(GLOBAL PROPERTY PHP_I_DISPLAY_FILES ${display_files})
endfunction()

# 将display文件进行显示
function(php_add_display_target)
   get_property(display_files GLOBAL PROPERTY PHP_I_DISPLAY_FILES)
   add_custom_target(display_targets SOURCES ${display_files})
endfunction()


# feature 需要探测的feature名称
# out_value 结果输出变量
macro(php_m_register_clang_feature feature_name out_value)
   string(STRIP "${feature_name}" feature_name)
   string(STRIP "${out_value}" out_value)
   if(feature_name STREQUAL "")
      message(FATAL_ERROR "feature_name can not be empty")
   endif()
   if(out_value STREQUAL "")
      message(FATAL_ERROR "out_value can not be empty")
   endif()
   set(PHP_INTERNAL_COMPILER_FEATURES ${PHP_INTERNAL_COMPILER_FEATURES} ${feature_name})
   set(PHP_INTERNAL_COMPILER_FEATURE_DETECT_OUT_VARS ${PHP_INTERNAL_COMPILER_FEATURE_DETECT_OUT_VARS} ${out_value})
endmacro()

macro(php_m_detect_clang_features)
   # 首先生成探测的cxx宏定义 TODO暂时不做合法性判断
   set(PHP_TEMP_FEATURE_DETECT_MACROS)
   list(LENGTH PHP_INTERNAL_COMPILER_FEATURES feature_length)
   if(feature_length GREATER 0)
      math(EXPR feature_length " ${feature_length} - 1")
      foreach(idx RANGE 0 ${feature_length})
         list(GET PHP_INTERNAL_COMPILER_FEATURES ${idx} cur_feature)
         list(GET PHP_INTERNAL_COMPILER_FEATURE_DETECT_OUT_VARS ${idx} cur_out_var)
         set(PHP_TEMP_FEATURE_DETECT_MACROS ${PHP_TEMP_FEATURE_DETECT_MACROS} "
            #if __has_feature(${cur_feature})
            featureStatus[\"${cur_out_var}\"] = true;
            #else
            featureStatus[\"${cur_out_var}\"] = false;
            #endif
            ")
      endforeach()
      configure_file(${PHP_CMAKE_MODULE_DIR}/DetectClangFeature.cpp.in
         ${CMAKE_BINARY_DIR}/CMakeTmp/DetectClangFeature.cpp)
      try_run(run_ret compiler_ret
         ${CMAKE_BINARY_DIR}
         ${CMAKE_BINARY_DIR}/CMakeTmp/DetectClangFeature.cpp
         COMPILE_DEFINITIONS ${PHP_COMMON_COMPILE_OPTS}
         RUN_OUTPUT_VARIABLE run_output_var
         COMPILE_OUTPUT_VARIABLE compiler_output_var
         )
      if(NOT run_ret EQUAL 0)
         message(FATAL_ERROR "runtime error")
      endif()
      foreach(cur_ret ${run_output_var})
         string(REPLACE ":" ";" cur_ret ${cur_ret})
         list(GET cur_ret 0 feature_output_varname)
         list(GET cur_ret 1 feature_flag)
         set(${feature_output_varname} ${feature_flag})
      endforeach()
   endif()
endmacro()