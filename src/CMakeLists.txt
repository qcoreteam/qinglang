#全局的相关变量设置

set(PHP_KERNEL_LIB_INCLUDES)
set(PHP_LANG_INCLUDES)
set(ZEND_ENGINE_INCLUDES)

#增加子项目库
add_subdirectory(kernel)
add_subdirectory(sapi)
add_subdirectory(utils)
add_subdirectory(language)
add_subdirectory(extension)
add_subdirectory(zend)