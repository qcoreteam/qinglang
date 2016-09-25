add_custom_target(UnitTests)
set_target_properties(UnitTests PROPERTIES FOLDER "Tests")
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)

function(php_m_add_unittest test_dirname)
  php_add_unittest(UnitTests ${test_dirname} ${ARGN})
endfunction()

add_subdirectory(kernel)
add_subdirectory(language)
add_subdirectory(zend)
add_subdirectory(sapi)
