# 将头文件都显示出来
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
php_add_file_to_display_pool(${headers})