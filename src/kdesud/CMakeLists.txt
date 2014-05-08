include(CheckFunctionExists)
check_function_exists(getpeereid HAVE_GETPEEREID) # openbsd style
check_function_exists(getpeereucred HAVE_GETPEERUCRED) # solaris style

configure_file (config-kdesud.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesud.h )


set(kdesud_SRCS
   kdesud.cpp
   repo.cpp
   lexer.cpp
   handler.cpp
   secure.cpp )

add_executable(kdesud ${kdesud_SRCS})
ecm_mark_nongui_executable(kdesud)

target_link_libraries(kdesud KF5::Su KF5::I18n ${X11_LIBRARIES})

if(KDE4_ENABLE_FPIE)
    macro_add_compile_flags(kdesud ${KDE4_CXX_FPIE_FLAGS})
    macro_add_link_flags(kdesud ${KDE4_PIE_LDFLAGS})
endif()

########### install files ###############

install(TARGETS kdesud DESTINATION ${KF5_LIBEXEC_INSTALL_DIR})
if(IS_ABSOLUTE KF5_LIBEXEC_INSTALL_DIR)
    set(ABS_KF5_LIBEXEC_INSTALL_DIR "${KF5_LIBEXEC_INSTALL_DIR}")
else()
    set(ABS_KF5_LIBEXEC_INSTALL_DIR "\${CMAKE_INSTALL_PREFIX}/${KF5_LIBEXEC_INSTALL_DIR}")
endif()
install(CODE "
    set(KDESUD_PATH \"\$ENV{DESTDIR}${ABS_KF5_LIBEXEC_INSTALL_DIR}/kdesud\")
    execute_process(COMMAND sh -c \"chgrp nogroup '\${KDESUD_PATH}' && chmod g+s '\${KDESUD_PATH}'\")
")

