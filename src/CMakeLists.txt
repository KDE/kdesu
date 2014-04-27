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

target_include_directories(KF5Su PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/..)

# Apps must include <KDESu/File> or <kdesu/file.h>
target_include_directories(KF5Su INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KDESu>")

target_link_libraries(KF5Su
  PUBLIC
    KF5::Pty
  PRIVATE
    KF5::CoreAddons # KUser::loginName
    KF5::Service # KToolInvocation::kdeinitExecWait
)

if(KDESU_USE_SUDO_DEFAULT)
   set_source_files_properties(su.cpp PROPERTIES 
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
install(FILES ${KDESu_CamelCase_HEADERS} DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KDESu/KDESu COMPONENT Devel)

install(TARGETS KF5Su EXPORT KF5SuTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

include(ECMMarkNonGuiExecutable)

add_executable(kdesu_stub kdesu_stub.c)
ecm_mark_nongui_executable(kdesu_stub)

install(TARGETS kdesu_stub DESTINATION ${KF5_LIBEXEC_INSTALL_DIR})

########### install files ###############
install(FILES
  su.h
  ssh.h
  stub.h
  process.h
  defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/kdesu/kdesu_export.h
  ${KDESu_HEADERS}
  DESTINATION  ${KF5_INCLUDE_INSTALL_DIR}/KDESu/kdesu COMPONENT Devel
)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDESu LIB_NAME KF5Su DEPS "KPty" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
