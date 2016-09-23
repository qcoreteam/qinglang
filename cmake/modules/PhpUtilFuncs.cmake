#这个文件定义一些常用的工具类

function(php_install_library_symlink name dest type)
   
endfunction()

function(php_install_symlink name dest)
   
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

function(php_add_header_files_for_glob hdrs_out glob)
endfunction()

function(php_find_all_header_files hdrs_out additional_headerdirs)
endfunction()

function(php_process_sources out_var)
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

macro(php_add_td_sources srcs)
endmacro()