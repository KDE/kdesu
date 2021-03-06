include(CheckFunctionExists)
check_function_exists(getpeereid HAVE_GETPEEREID) # openbsd style
check_function_exists(getpeereucred HAVE_GETPEERUCRED) # solaris style

configure_file (config-kdesud.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesud.h )

add_executable(kdesud)
ecm_mark_nongui_executable(kdesud)

target_sources(kdesud PRIVATE
   kdesud.cpp
   repo.cpp
   lexer.cpp
   handler.cpp
   secure.cpp
)

ecm_qt_declare_logging_category(kdesud
    HEADER ksud_debug.h
    IDENTIFIER KSUD_LOG
    CATEGORY_NAME kf.su.kdesud
    DESCRIPTION "kdesud (KDESu)"
    EXPORT KSU
)

target_link_libraries(kdesud KF5::Su KF5::I18n ${X11_LIBRARIES})
if(HAVE_X11)
  target_include_directories(kdesud PRIVATE ${X11_X11_INCLUDE_PATH})
endif()

if(KDE4_ENABLE_FPIE)
    macro_add_compile_flags(kdesud ${KDE4_CXX_FPIE_FLAGS})
    macro_add_link_flags(kdesud ${KDE4_PIE_LDFLAGS})
endif()

if(BUILD_TESTING)
  add_subdirectory(autotests)
endif()

########### install files ###############

install(TARGETS kdesud DESTINATION ${KDE_INSTALL_LIBEXECDIR_KF5})
install(CODE "
    set(KDESUD_PATH \"\$ENV{DESTDIR}${KDE_INSTALL_FULL_LIBEXECDIR_KF5}/kdesud\")
    execute_process(COMMAND sh -c \"chgrp nogroup '\${KDESUD_PATH}' && chmod g+s '\${KDESUD_PATH}'\")
")

