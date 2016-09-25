project(phpkernel)
# 文件列表设置
set(PHP_KERNEL_LIB_SRCS)
# 在这些cmake脚本里面只是为了添加本目录的文件
include(base/files_register.cmake)
include(cli/files_register.cmake)
include(mmgr/files_register.cmake)
include(os/files_register.cmake)

php_m_add_library(phpkernel ${PHP_KERNEL_LIB_SRCS})
#add_library(phpkernel ${PHP_KERNEL_LIB_SRCS})
# 文件安装




