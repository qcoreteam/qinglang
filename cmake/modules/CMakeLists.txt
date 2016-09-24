#这个文件主要处理cmake modules文件安装
set(PHP_INSTALL_PACKAGE_DIR lib${PHP_LIBDIR_SUFFIX}/cmake/php)
set(php_cmake_builddir "${PHP_BINARY_DIR}/${PHP_INSTALL_PACKAGE_DIR}")

set(PHP_EXPORTS_FILE ${php_cmake_builddir}/PhpExports.cmake)
get_property(PHP_EXPORTS GLOBAL PROPERTY PHP_EXPORTS)
export(TARGETS ${PHP_EXPORTS} FILE ${PHP_EXPORTS_FILE})

get_property(PHP_AVAILABLE_LIBS GLOBAL PROPERTY PHP_LIBS)

foreach(lib ${PHP_AVAILABLE_LIBS})
  get_property(php_lib_deps GLOBAL PROPERTY PHPBUILD_LIB_DEPS_${lib})
  set(all_php_lib_deps
    "${all_php_lib_deps}\nset_property(GLOBAL PROPERTY PHPBUILD_LIB_DEPS_${lib} ${php_lib_deps})")
endforeach(lib)

set(PHP_CONFIG_CODE "
# PHP_BUILD_* values available only from PHP build tree.
set(PHP_BUILD_BINARY_DIR \"${PHP_BINARY_DIR}\")
set(PHP_BUILD_LIBRARY_DIR \"${PHP_LIBRARY_DIR}\")
set(PHP_BUILD_MAIN_INCLUDE_DIR \"${PHP_MAIN_INCLUDE_DIR}\")
set(PHP_BUILD_MAIN_SRC_DIR \"${PHP_MAIN_SRC_DIR}\")
")

set(PHP_CONFIG_INCLUDE_DIRS
   "${PHP_MAIN_INCLUDE_DIR}"
   "${PHP_BINARY_INCLUDE_DIR}")

set(PHP_CONFIG_LIBRARY_DIRS
   "${PHP_LIBRARY_DIR}")

set(PHP_CONFIG_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PHP_CONFIG_BINARY_DIR "${PHP_BINARY_DIR}")

set(PHP_CONFIG_EXPORTS_FILE "${PHP_EXPORTS_FILE}")
set(PHP_CONFIG_EXPORTS "${PHP_EXPORTS};${PHP_EXPORTS_BUILDTREE_ONLY}")

configure_file(
  PhpConfig.cmake.in
  ${php_cmake_builddir}/PhpConfig.cmake
  @ONLY)

set(PHP_CONFIG_CODE "
# Compute the installation prefix from this LLVMConfig.cmake file location.
get_filename_component(PHP_INSTALL_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)")

string(REGEX REPLACE "/" ";" _count "${PHP_INSTALL_PACKAGE_DIR}")

foreach(p ${_count})
  set(PHP_CONFIG_CODE "${PHP_CONFIG_CODE}
get_filename_component(PHP_INSTALL_PREFIX \"\${PHP_INSTALL_PREFIX}\" PATH)")
endforeach()

set(PHP_CONFIG_INCLUDE_DIRS "\${PHP_INSTALL_PREFIX}/include")
set(PHP_CONFIG_LIBRARY_DIRS "\${PHP_INSTALL_PREFIX}/lib\${LLVM_LIBDIR_SUFFIX}")
set(PHP_CONFIG_CMAKE_DIR "\${PHP_INSTALL_PREFIX}/${LLVM_INSTALL_PACKAGE_DIR}")
set(PHP_CONFIG_BINARY_DIR "\${PHP_INSTALL_PREFIX}")
set(PHP_CONFIG_TOOLS_BINARY_DIR "\${PHP_INSTALL_PREFIX}/bin")
set(PHP_CONFIG_EXPORTS_FILE "\${PHP_CMAKE_DIR}/PhpExports.cmake")

configure_file(
  PhpConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/PhpConfig.cmake
  @ONLY)

configure_file(
  PhpConfigVersion.cmake.in
  ${php_cmake_builddir}/PhpConfigVersion.cmake
  @ONLY)

if (PHP_OPT_INSTALL_SUPPORT_FILES)
  install(EXPORT PhpExports DESTINATION ${PHP_INSTALL_PACKAGE_DIR})
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/PhpConfig.cmake
    ${php_cmake_builddir}/PhpConfigVersion.cmake
    PhpCfgFuncs.cmake
    DESTINATION ${PHP_INSTALL_PACKAGE_DIR})
 
  install(DIRECTORY .
    DESTINATION ${PHP_INSTALL_PACKAGE_DIR}
    FILES_MATCHING PATTERN *.cmake
    PATTERN .svn EXCLUDE
    PATTERN PhpConfig.cmake EXCLUDE
    PATTERN PhpConfigVersion.cmake EXCLUDE
    PATTERN PhpCfgFuncs.cmake EXCLUDE)
endif()