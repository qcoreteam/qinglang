# 我们需要在安装阶段运行这个脚本，因为DESTDIR等变量可能在configure阶段不可用
function(install_symlink name target outdir)
  if(UNIX)
    set(LINK_OR_COPY create_symlink)
    set(DESTDIR $ENV{DESTDIR})
  else()
    set(LINK_OR_COPY copy)
  endif()
  set(bindir "${DESTDIR}${CMAKE_INSTALL_PREFIX}/${outdir}/")
  message("Creating ${name}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E ${LINK_OR_COPY} "${target}" "${name}"
    WORKING_DIRECTORY "${bindir}")
endfunction()