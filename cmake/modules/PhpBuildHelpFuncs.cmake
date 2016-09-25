# 这个文件定义一些跟编译相关的帮助函数
# 通用函数定义

function(php_add_symbol_exports target_name export_file)
   if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set(native_export_file "${target_name}.exports")
      add_custom_command(OUTPUT ${native_export_file}
         COMMAND sed -e "s/^/_/" < ${export_file} > ${native_export_file}
         DEPENDS ${export_file}
         VERBATIM
         COMMENT "Creating export file for ${target_name}")
      set_property(TARGET ${target_name} APPEND_STRING PROPERTY
         LINK_FLAGS " -Wl,-exported_symbols_list,${CMAKE_CURRENT_BINARY_DIR}/${native_export_file}")
   elseif(${CMAKE_SYSTEM_NAME} MATCHES "AIX")
      set_property(TARGET ${target_name} APPEND_STRING PROPERTY
         LINK_FLAGS " -Wl,-bE:${export_file}")
   elseif(PHP_HAVE_LINK_VERSION_SCRIPT)
      # Gold and BFD ld require a version script rather than a plain list.
      set(native_export_file "${target_name}.exports")
      # FIXME: Don't write the "local:" line on OpenBSD.
      add_custom_command(OUTPUT ${native_export_file}
         COMMAND echo "{" > ${native_export_file}
         COMMAND grep -q "[[:alnum:]]" ${export_file} && echo "  global:" >> ${native_export_file} || :
         COMMAND sed -e "s/$/;/" -e "s/^/    /" < ${export_file} >> ${native_export_file}
         COMMAND echo "  local: *;" >> ${native_export_file}
         COMMAND echo "};" >> ${native_export_file}
         DEPENDS ${export_file}
         VERBATIM
         COMMENT "Creating export file for ${target_name}")
      if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
         set_property(TARGET ${target_name} APPEND_STRING PROPERTY
            LINK_FLAGS "  -Wl,-M,${CMAKE_CURRENT_BINARY_DIR}/${native_export_file}")
      else()
         set_property(TARGET ${target_name} APPEND_STRING PROPERTY
            LINK_FLAGS "  -Wl,--version-script,${CMAKE_CURRENT_BINARY_DIR}/${native_export_file}")
      endif()
   else()
      set(native_export_file "${target_name}.def")
      add_custom_command(OUTPUT ${native_export_file}
         COMMAND ${PYTHON_EXECUTABLE} -c "import sys;print(''.join(['EXPORTS\\n']+sys.stdin.readlines(),))"
         < ${export_file} > ${native_export_file}
         DEPENDS ${export_file}
         VERBATIM
         COMMENT "Creating export file for ${target_name}")
      set(export_file_linker_flag "${CMAKE_CURRENT_BINARY_DIR}/${native_export_file}")
      if(MSVC)
         set(export_file_linker_flag "/DEF:\"${export_file_linker_flag}\"")
      endif()
      set_property(TARGET ${target_name} APPEND_STRING PROPERTY
         LINK_FLAGS " ${export_file_linker_flag}")
   endif()
   add_custom_target(${target_name}_exports DEPENDS ${native_export_file})
   set_target_properties(${target_name}_exports PROPERTIES FOLDER "Misc")
   get_property(srcs TARGET ${target_name} PROPERTY SOURCES)
   foreach(src ${srcs})
      get_filename_component(extension ${src} EXT)
      if(extension STREQUAL ".cpp")
         set(first_source_file ${src})
         break()
      endif()
   endforeach()
   set_property(DIRECTORY APPEND
      PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${native_export_file})
   add_dependencies(${target_name} ${target_name}_exports)
   # Add dependency to *_exports later -- CMake issue 14747
   list(APPEND PHP_COMMON_DEPENDS ${target_name}_exports)
   set(PHP_COMMON_DEPENDS ${PHP_COMMON_DEPENDS} PARENT_SCOPE)
endfunction()

if(NOT WIN32 AND NOT APPLE)
   execute_process(
      COMMAND ${CMAKE_C_COMPILER} -Wl,--version
      OUTPUT_VARIABLE stdout
      ERROR_QUIET
      )
   if("${stdout}" MATCHES "GNU gold")
      set(PHP_LINKER_IS_GOLD ON)
   endif()
endif()

function(php_add_link_optimizations target_name)
   # 不要在debug编译模式下使用linker优化参数，因为会使编译速度变慢，而且在debug模式中优化也没有任何意义
   string(TOUPPER "${CMAKE_BUILD_TYPE}" build_type)
   if(NOT build_type STREQUAL "DEBUG")
      # Pass -O3 to the linker. This enabled different optimizations on different
      # linkers.
      if(NOT (${CMAKE_SYSTEM_NAME} MATCHES "Darwin|SunOS|AIX" OR WIN32))
         set_property(TARGET ${target_name} APPEND_STRING PROPERTY
            LINK_FLAGS " -Wl,-O3")
      endif()
      if(LLVM_LINKER_IS_GOLD)
         # With gold gc-sections is always safe.
         set_property(TARGET ${target_name} APPEND_STRING PROPERTY
            LINK_FLAGS " -Wl,--gc-sections")
         # Note that there is a bug with -Wl,--icf=safe so it is not safe
         # to enable. See https://sourceware.org/bugzilla/show_bug.cgi?id=17704.
      endif()
      if(NOT PHP_NO_DEAD_STRIP)
         if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            # ld64's implementation of -dead_strip breaks tools that use plugins.
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY
               LINK_FLAGS " -Wl,-dead_strip")
         elseif(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY
               LINK_FLAGS " -Wl,-z -Wl,discard-unused=sections")
         elseif(NOT WIN32 AND NOT LLVM_LINKER_IS_GOLD)
            # Object files are compiled with -ffunction-data-sections.
            # Versions of bfd ld < 2.23.1 have a bug in --gc-sections that breaks
            # tools that use plugins. Always pass --gc-sections once we require
            # a newer linker.
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY
               LINK_FLAGS " -Wl,--gc-sections")
         endif()
      endif()
   endif()
endfunction()

# 根据${CMAKE_CONFIGURATION_TYPES}的值设置target的输出文件夹.
# 提示：不要直接的设置CMAKE_*_OUTPUT_DIRECTORY等cmake变量，或者一个特定的builder，例如msbuild.exe，可能会混淆
function(php_set_output_directory target)
   cmake_parse_arguments(PHP_ARG "BINARY_DIR;LIBRARY_DIR" "" "" ${ARGN})
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


# 更新目标的编译flag
# name target名称
# PHP_TEMP_COMPILE_FLAGS 需要添加的编译器标志
# PHP_TEMP_COMPILE_DEFINITIONS 需要添加的编译器定义
function(php_update_compile_flags name)
   get_property(sources TARGET ${name} PROPERTY SOURCES)
   if("${sources}" MATCHES "\\.c(;|$)")
      set(update_src_props ON)
   endif()
   # LLVM_REQUIRES_EH is an internal flag that individual targets can use to
   # force EH
   if(PHP_REQUIRES_EH OR PHP_ENABLE_EH)
      if(NOT (PHP_REQUIRES_RTTI OR PHP_ENABLE_RTTI))
         message(AUTHOR_WARNING "Exception handling requires RTTI. Enabling RTTI for ${name}")
         set(PHP_REQUIRES_RTTI ON)
      endif()
      if(MSVC)
         list(APPEND PHP_COMPILE_FLAGS "/EHsc")
      endif()
   else()
      if(PHP_COMPILER_IS_GCC_COMPATIBLE)
         list(APPEND PHP_COMPILE_FLAGS "-fno-exceptions")
      elseif(MSVC)
         list(APPEND PHP_COMPILE_DEFINITIONS _HAS_EXCEPTIONS=0)
         list(APPEND PHP_COMPILE_FLAGS "/EHs-c-")
      endif()
   endif()
   # LLVM_REQUIRES_RTTI is an internal flag that individual
   # targets can use to force RTTI
   set(PHP_CONFIG_HAS_RTTI YES CACHE INTERNAL "")
   if(NOT (PHP_REQUIRES_RTTI OR PHP_ENABLE_RTTI))
      set(PHP_CONFIG_HAS_RTTI NO CACHE INTERNAL "")
      list(APPEND PHP_COMPILE_DEFINITIONS GTEST_HAS_RTTI=0)
      if (PHP_COMPILER_IS_GCC_COMPATIBLE)
         list(APPEND PHP_COMPILE_FLAGS "-fno-rtti")
      elseif (MSVC)
         list(APPEND PHP_COMPILE_FLAGS "/GR-")
      endif ()
   elseif(MSVC)
      list(APPEND PHP_COMPILE_FLAGS "/GR")
   endif()
   # Assume that;
   #   - LLVM_COMPILE_FLAGS is list.
   #   - PROPERTY COMPILE_FLAGS is string.
   string(REPLACE ";" " " target_compile_flags " ${PHP_TEMP_COMPILE_FLAGS}")
   if(update_src_props)
      foreach(fn ${sources})
         get_filename_component(suf ${fn} EXT)
         if("${suf}" STREQUAL ".cpp")
            set_property(SOURCE ${fn} APPEND_STRING PROPERTY
               COMPILE_FLAGS "${target_compile_flags}")
         endif()
      endforeach()
   else()
      # Update target props, since all sources are C++.
      set_property(TARGET ${name} APPEND_STRING PROPERTY
         COMPILE_FLAGS "${target_compile_flags}")
   endif()
   set_property(TARGET ${name} APPEND PROPERTY COMPILE_DEFINITIONS ${PHP_TEMP_COMPILE_DEFINITIONS})
endfunction()

# php_add_library(name sources...
#   SHARED;STATIC
#     STATIC by default w/o BUILD_SHARED_LIBS.
#     SHARED by default w/  BUILD_SHARED_LIBS.
#   OBJECT
#     Also create an OBJECT library target. Default if STATIC && SHARED.
#   MODULE
#     Target ${name} might not be created on unsupported platforms.
#     Check with "if(TARGET ${name})".
#   DISABLE_PHP_LINK_PHP_DYLIB
#     Do not link this library to libLLVM, even if
#     PHP_LINK_PHP_DYLIB is enabled.
#   OUTPUT_NAME name
#     Corresponds to OUTPUT_NAME in target properties.
#   DEPENDS targets...
#     Same semantics as add_dependencies().
#   LINK_COMPONENTS components...
#     Same as the variable PHP_LINK_COMPONENTS.
#   LINK_LIBS lib_targets...
#     Same semantics as target_link_libraries().
#   ADDITIONAL_HEADERS
#     May specify header files for IDE generators.
#   SONAME
#     Should set SONAME link flags and create symlinks
#   )
function(php_add_library name)
   cmake_parse_arguments(PHP_ARG
      "MODULE;SHARED;STATIC;OBJECT;DISABLE_PHP_LINK_PHP_DYLIB;SONAME"
      "OUTPUT_NAME"
      "ADDITIONAL_HEADERS;DEPENDS;LINK_COMPONENTS;LINK_LIBS;OBJLIBS"
      ${ARGN})
   list(APPEND PHP_COMMON_DEPENDS ${PHP_ARG_DEPENDS})
   if(PHP_ARG_ADDITIONAL_HEADERS)
      # Pass through ADDITIONAL_HEADERS.
      set(PHP_ARG_ADDITIONAL_HEADERS ADDITIONAL_HEADERS ${PHP_ARG_ADDITIONAL_HEADERS})
   endif()
   if(PHP_ARG_OBJLIBS)
      set(ALL_FILES ${PHP_ARG_OBJLIBS})
   else()
      php_process_sources(ALL_FILES ${PHP_ARG_UNPARSED_ARGUMENTS} ${PHP_ARG_ADDITIONAL_HEADERS})
   endif()
   if(PHP_ARG_MODULE)
      if(PHP_ARG_SHARED OR PHP_ARG_STATIC)
         message(WARNING "MODULE with SHARED|STATIC doesn't make sense.")
      endif()
   else()
      if(PHP_ARG_PLUGIN_TOOL)
         message(WARNING "PLUGIN_TOOL without MODULE doesn't make sense.")
      endif()
      if(BUILD_SHARED_LIBS AND NOT PHP_ARG_STATIC)
         set(PHP_ARG_SHARED TRUE)
      endif()
      if(NOT PHP_ARG_SHARED)
         set(PHP_ARG_STATIC TRUE)
      endif()
   endif()
   # Generate objlib
   if((PHP_ARG_SHARED AND PHP_ARG_STATIC) OR PHP_ARG_OBJECT)
      # Generate an obj library for both targets.
      set(obj_name "obj.${name}")
      add_library(${obj_name} OBJECT EXCLUDE_FROM_ALL
         ${ALL_FILES}
         )
      php_update_compile_flags(${obj_name})
      set(ALL_FILES "$<TARGET_OBJECTS:${obj_name}>")
      # Do add_dependencies(obj) later due to CMake issue 14747.
      list(APPEND objlibs ${obj_name})
      set_target_properties(${obj_name} PROPERTIES FOLDER "Object Libraries")
   endif()
   if(PHP_ARG_SHARED AND PHP_ARG_STATIC)
      # static
      set(name_static "${name}_static")
      if(PHP_ARG_OUTPUT_NAME)
         set(output_name OUTPUT_NAME "${PHP_ARG_OUTPUT_NAME}")
      endif()
      # DEPENDS has been appended to LLVM_COMMON_LIBS.
      php_add_library(${name_static} STATIC
         ${output_name}
         OBJLIBS ${ALL_FILES} # objlib
         LINK_LIBS ${PHP_ARG_LINK_LIBS}
         LINK_COMPONENTS ${PHP_ARG_LINK_COMPONENTS}
         )
      # FIXME: Add name_static to anywhere in TARGET ${name}'s PROPERTY.
      set(PHP_ARG_STATIC)
   endif()
   if(PHP_ARG_MODULE)
      add_library(${name} MODULE ${ALL_FILES})
   elseif(PHP_ARG_SHARED)
      add_library(${name} SHARED ${ALL_FILES})
   else()
      add_library(${name} STATIC ${ALL_FILES})
   endif()
   php_set_output_directory(${name} BINARY_DIR ${PHP_RUNTIME_OUTPUT_INTDIR} LIBRARY_DIR ${PHP_LIBRARY_OUTPUT_INTDIR})
   # $<TARGET_OBJECTS> doesn't require compile flags.
   if(NOT obj_name)
      php_update_compile_flags(${name})
   endif()
   php_add_link_optimizations(${name})
   if(PHP_ARG_OUTPUT_NAME)
      set_target_properties(${name}
         PROPERTIES
         OUTPUT_NAME ${PHP_ARG_OUTPUT_NAME}
         )
   endif()
   
   if(PHP_ARG_MODULE)
      set_target_properties(${name} PROPERTIES
         PREFIX ""
         SUFFIX ${PHP_PLUGIN_EXT}
         )
   endif()
   
   if(PHP_ARG_SHARED)
      if(WIN32)
         set_target_properties(${name} PROPERTIES
            PREFIX ""
            )
      endif()
   endif()
   if(PHP_ARG_MODULE OR PHP_ARG_SHARED)
      # Do not add -Dname_EXPORTS to the command-line when building files in this
      # target. Doing so is actively harmful for the modules build because it
      # creates extra module variants, and not useful because we don't use these
      # macros.
      set_target_properties(${name} PROPERTIES DEFINE_SYMBOL "")
      if (PHP_EXPORTED_SYMBOL_FILE)
         php_add_symbol_exports(${name} ${PHP_EXPORTED_SYMBOL_FILE})
      endif()
   endif()
   if(PHP_ARG_SHARED AND UNIX)
      if(NOT APPLE AND PHP_ARG_SONAME)
         get_target_property(output_name ${name} OUTPUT_NAME)
         if(${output_name} STREQUAL "output_name-NOTFOUND")
            set(output_name ${name})
         endif()
         set(library_name ${output_name}-${PHP_VERSION_MAJOR}.${PHP_VERSION_MINOR}${PHP_VERSION_STAGE})
         set(api_name ${output_name}-${PHP_VERSION_MAJOR}.${PHP_VERSION_MINOR}.${PHP_VERSION_PATCH}${PHP_VERSION_STAGE})
         set_target_properties(${name} PROPERTIES OUTPUT_NAME ${library_name})
         php_install_library_symlink(${api_name} ${library_name} SHARED
            COMPONENT ${name}
            ALWAYS_GENERATE)
         php_install_library_symlink(${output_name} ${library_name} SHARED
            COMPONENT ${name}
            ALWAYS_GENERATE)
      endif()
   endif()
   if(PHP_ARG_STATIC)
      set(libtype INTERFACE)
   else()
      # We can use PRIVATE since SO knows its dependent libs.
      set(libtype PRIVATE)
   endif()
   target_link_libraries(${name} ${libtype}
      ${PHP_ARG_LINK_LIBS}
      ${lib_deps}
      ${php_libs}
      )
   
   if(PHP_COMMON_DEPENDS)
      add_dependencies(${name} ${PHP_COMMON_DEPENDS})
      # Add dependencies also to objlibs.
      # CMake issue 14747 --  add_dependencies() might be ignored to objlib's user.
      foreach(objlib ${objlibs})
         add_dependencies(${objlib} ${PHP_COMMON_DEPENDS})
      endforeach()
   endif()
endfunction()

# 增加一个单元测试
# test_suite 测试集合名称，通常是一个大的模块放在一起然后给一个名字
# test_name 测试集合里面的单个测试名字
function(php_add_unittest test_suite test_name)
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
   php_m_add_executable(${test_name} ${ARGN})
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

# 相关宏定义
# PHP_TEMP_EXCLUDE_FROM_ALL会读取环境里面的值
# ... 为文件列表
macro(php_m_add_library name)
   cmake_parse_arguments(PHP_ARG
      "SHARED;BUILDTREE_ONLY"
      ""
      ""
      ${ARGN})
   if(BUILD_SHARED_LIBS OR PHP_ARG_SHARED)
      php_add_library(${name} SHARED ${PHP_ARG_UNPARSED_ARGUMENTS})
   else()
      php_add_library(${name} ${PHP_ARG_UNPARSED_ARGUMENTS})
   endif()
   # Libraries that are meant to only be exposed via the build tree only are
   # never installed and are only exported as a target in the special build tree
   # config file.
   if (NOT PHP_ARG_BUILDTREE_ONLY)
      set_property(GLOBAL APPEND PROPERTY PHP_LIBS ${name})
   endif()
   if(PHP_TEMP_EXCLUDE_FROM_ALL)
      set_target_properties(${name} PROPERTIES EXCLUDE_FROM_ALL ON)
   elseif(PHP_ARG_BUILDTREE_ONLY)
      set_property(GLOBAL APPEND PROPERTY PHP_EXPORTS_BUILDTREE_ONLY ${name})
   endif()
   set_property(GLOBAL APPEND PROPERTY PHP_EXPORTS ${name})
   set_target_properties(${name} PROPERTIES FOLDER "Libraries")
endmacro(php_m_add_library)

macro(php_m_add_loadable_module)
   
endmacro()


# PHP_TEMP_ENABLE_OBJLIB 环境读取
# PHP_TEMP_EXCLUDE_FROM_ALL
macro(php_m_add_executable name)
   cmake_parse_arguments(PHP_ARG "" "" "" ${ARGN})
   php_process_sources(ALL_FILES ${PHP_ARG_UNPARSED_ARGUMENTS})
   # Generate objlib
   if(PHP_TEMP_ENABLE_OBJLIB)
      # Generate an obj library for both targets.
      set(obj_name "obj.${name}")
      add_library(${obj_name} OBJECT EXCLUDE_FROM_ALL
         ${ALL_FILES}
         )
      php_update_compile_flags(${obj_name})
      set(ALL_FILES "$<TARGET_OBJECTS:${obj_name}>")
      set_target_properties(${obj_name} PROPERTIES FOLDER "Object Libraries")
   endif()
   if(XCODE)
      # Note: the dummy.cpp source file provides no definitions. However,
      # it forces Xcode to properly link the static library.
      list(APPEND ALL_FILES "${PHP_MAIN_SRC_DIR}/cmake/dummy.cpp")
   endif()
   if(PHP_TEMP_EXCLUDE_FROM_ALL)
      add_executable(${name} EXCLUDE_FROM_ALL ${ALL_FILES})
   else()
      add_executable(${name} ${ALL_FILES})
   endif()
   # $<TARGET_OBJECTS> doesn't require compile flags.
   if(NOT PHP_TEMP_ENABLE_OBJLIB)
      php_update_compile_flags(${name})
   endif()
   php_add_link_optimizations(${name})
   # Do not add -Dname_EXPORTS to the command-line when building files in this
   # target. Doing so is actively harmful for the modules build because it
   # creates extra module variants, and not useful because we don't use these
   # macros.
   set_target_properties(${name} PROPERTIES DEFINE_SYMBOL "")
   if (PHP_TEMP_EXPORTED_SYMBOL_FILE)
      php_add_symbol_exports(${name} ${PHP_TEMP_EXPORTED_SYMBOL_FILE})
   endif(PHP_TEMP_EXPORTED_SYMBOL_FILE)
   set(PHP_TEMP_EXCLUDE_FROM_ALL OFF)
   php_set_output_directory(${name} BINARY_DIR ${PHP_RUNTIME_OUTPUT_INTDIR} LIBRARY_DIR ${PHP_LIBRARY_OUTPUT_INTDIR})
   if(PHP_COMMON_DEPENDS)
      add_dependencies(${name} ${PHP_COMMON_DEPENDS})
   endif(PHP_COMMON_DEPENDS)
   if (PHP_PTHREAD_LIB)
      # libpthreads overrides some standard library symbols, so main
      # executable must be linked with it in order to provide consistent
      # API for all shared libaries loaded by this executable.
      target_link_libraries(${name} ${PHP_PTHREAD_LIB})
   endif()
endmacro()

# cmake系统中没有一种方法获取当前已经添加的编译器定义，这个宏我们自己实现相关的功能
# 参数是跟add_definitions一致
macro(php_m_add_definitions)
   foreach(arg ${ARGN})
      if(DEFINED PHP_DEFINITIONS)
         set(PHP_DEFINITIONS "${PHP_DEFINITIONS} ${arg}")
      elseif()
         set(PHP_DEFINITIONS ${arg})
      endif(DEFINED PHP_DEFINITIONS)
   endforeach()
   add_definitions(${ARGN})
endmacro(php_m_add_definitions)


macro(php_m_add_target target_name)
   include_directories(BEFORE
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR})
   php_m_add_library(Php${target_name} ${ARGN})
   set(CURRENT_PHP_TARGET Php${target_name})
endmacro()