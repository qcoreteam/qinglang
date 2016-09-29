project(phpkernel)
# 文件列表设置
set(PHP_KERNEL_LIB_SRCS)
# 在这些cmake脚本里面只是为了添加本目录的文件
include(base/files_register.cmake)
include(cli/files_register.cmake)
include(mmgr/files_register.cmake)
include(os/files_register.cmake)
include(ds/files_register.cmake)
file(GLOB_RECURSE headers "${PHP_MAIN_INCLUDE_DIR}/php/kernel/*.h")

php_m_add_library(phpkernel ${PHP_KERNEL_LIB_SRCS} ${headers})
#add_library(phpkernel ${PHP_KERNEL_LIB_SRCS})
# 文件安装




