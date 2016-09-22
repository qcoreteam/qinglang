#全局的相关变量设置

set(PHP_KERNEL_LIB_API_HEADERS)
set(PHP_LANG_API_HEADERS)
set(ZEND_ENGINE_API_HEADERS)

#增加子项目库
add_subdirectory(kernel)
add_subdirectory(sapi)
add_subdirectory(utils)
add_subdirectory(language)
add_subdirectory(extension)
add_subdirectory(zend)