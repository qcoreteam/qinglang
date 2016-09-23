#这个文件定义一些常用的工具类

function(php_install_library_symlink name dest type)
   
endfunction()

function(php_install_symlink name dest)
   
endfunction()

function(llvm_replace_compiler_option var old new)
  # Replaces a compiler option or switch `old' in `var' by `new'.
  # If `old' is not in `var', appends `new' to `var'.
  # Example: llvm_replace_compiler_option(CMAKE_CXX_FLAGS_RELEASE "-O3" "-O2")
  # If the option already is on the variable, don't add it:
  if( "${${var}}" MATCHES "(^| )${new}($| )" )
    set(n "")
  else()
    set(n "${new}")
  endif()
  if( "${${var}}" MATCHES "(^| )${old}($| )" )
    string( REGEX REPLACE "(^| )${old}($| )" " ${n} " ${var} "${${var}}" )
  else()
    set( ${var} "${${var}} ${n}" )
  endif()
  set( ${var} "${${var}}" PARENT_SCOPE )
endfunction(llvm_replace_compiler_option)


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

function(php_check_source_file_list)
endfunction()

macro(php_add_td_sources srcs)
endmacro()