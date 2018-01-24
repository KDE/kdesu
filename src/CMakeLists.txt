add_subdirectory(kdesud)

include(CheckTypeSize)
include(CheckIncludeFiles)
include(CheckFunctionExists)

check_function_exists(setpriority HAVE_SETPRIORITY)
check_function_exists(getpeereid HAVE_GETPEEREID)
check_function_exists(initgroups HAVE_INITGROUPS)

check_include_files(sys/select.h  HAVE_SYS_SELECT_H)

set(CMAKE_EXTRA_INCLUDE_FILES sys/socket.h)
check_type_size("struct ucred" STRUCT_UCRED) #defines HAVE_STRUCT_UCRED (bool) and STRUCT_UCRED (size, unused)
set(CMAKE_EXTRA_INCLUDE_FILES)  #reset CMAKE_EXTRA_INCLUDE_FILES

configure_file(config-kdesu.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesu.h)

set(kdesu_LIB_SRCS
  client.cpp
  ptyprocess.cpp
  kcookie.cpp
  suprocess.cpp
  sshprocess.cpp
  stubprocess.cpp
)


add_library(KF5Su ${kdesu_LIB_SRCS})
generate_export_header(KF5Su BASE_NAME KDESu EXPORT_FILE_NAME kdesu/kdesu_export.h)
add_library(KF5::Su ALIAS KF5Su)

option(KDESU_USE_SUDO_DEFAULT "Set On to use sudo instead of su [default=Off]" Off)
if(KDESU_USE_SUDO_DEFAULT)
  message("Using sudo as default")
  target_compile_definitions(KF5Su PRIVATE -DKDESU_USE_SUDO_DEFAULT="true")
else()
  message("Using su as default (set KDESU_USE_SUDO_DEFAULT=On to use sudo)")
endif()
target_include_directories(KF5Su PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/..)

# Apps must include <KDESu/File> or <kdesu/file.h>
target_include_directories(KF5Su INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDESu>")

target_link_libraries(KF5Su
  PUBLIC
    KF5::Pty
  PRIVATE
    KF5::CoreAddons # KUser::loginName
    KF5::Service # KToolInvocation::kdeinitExecWait
)

if(KDESU_USE_SUDO_DEFAULT)
   set_source_files_properties(suprocess.cpp PROPERTIES 
                               COMPILE_FLAGS -DKDESU_USE_SUDO_DEFAULT)
endif()

set_target_properties(KF5Su PROPERTIES
  VERSION ${KDESU_VERSION_STRING}
  SOVERSION ${KDESU_SOVERSION}
  EXPORT_NAME Su
)

ecm_generate_headers(KDESu_CamelCase_HEADERS
  HEADER_NAMES
  PtyProcess
  SuProcess
  SshProcess
  StubProcess
  Client # technically the class is called KDEsuClient...

  PREFIX KDESu
  REQUIRED_HEADERS KDESu_HEADERS
)
install(FILES ${KDESu_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDESu/KDESu COMPONENT Devel)

install(TARGETS KF5Su EXPORT KF5SuTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

include(ECMMarkNonGuiExecutable)

set_source_files_properties(kdesu_stub.c PROPERTIES
                            COMPILE_DEFINITIONS "_XOPEN_SOURCE;_BSD_SOURCE")
add_executable(kdesu_stub kdesu_stub.c)
ecm_mark_nongui_executable(kdesu_stub)

install(TARGETS kdesu_stub DESTINATION ${KDE_INSTALL_LIBEXECDIR_KF5})

########### install files ###############
install(FILES
  su.h
  ssh.h
  stub.h
  process.h
  defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/kdesu/kdesu_export.h
  ${KDESu_HEADERS}
  DESTINATION  ${KDE_INSTALL_INCLUDEDIR_KF5}/KDESu/kdesu COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5Su_QCH
        NAME KDESu
        BASE_NAME KF5Su
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KDESu_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            KF5Pty_QCH
        BLANK_MACROS
            KDESU_EXPORT
            KDESU_DEPRECATED
            KDESU_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDESu LIB_NAME KF5Su DEPS "KPty" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KDESu)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
