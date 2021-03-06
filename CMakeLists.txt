cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.85.0") # handled by release scripts
set(KF_DEP_VERSION "5.85.0") # handled by release scripts
project(KDESu VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.85.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(REQUIRED_QT_VERSION 5.15.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

find_package(KF5CoreAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5I18n ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Service ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Pty ${KF_DEP_VERSION} REQUIRED)

#optional features
find_package(X11)
set(HAVE_X11 ${X11_FOUND})


include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMMarkNonGuiExecutable)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

option(KDESU_USE_SUDO_DEFAULT "Set On to use sudo instead of su [default=Off]" Off)
option(KDESU_USE_DOAS_DEFAULT "Set On to use doas instead of su and sudo [default=Off]" Off)
if(KDESU_USE_SUDO_DEFAULT AND KDESU_USE_DOAS_DEFAULT)
   message(FATAL_ERROR "Choose between sudo(1) or doas(1)")
elseif(KDESU_USE_SUDO_DEFAULT)
  message("Using sudo as default")
elseif(KDESU_USE_DOAS_DEFAULT)
  message("Using doas as default")
else()
  message("Using su as default (set KDESU_USE_SUDO_DEFAULT=On to use sudo)")
endif()

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX KDESU
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdesu_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5SuConfigVersion.cmake"
                        SOVERSION 5)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055100)
ki18n_install(po)
add_subdirectory(src)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Su")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5Su_QCH
        FILE KF5SuQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5SuQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5SuConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5SuConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5SuConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5SuConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5SuTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5SuTargets.cmake NAMESPACE KF5:: )

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/kdesu_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
)

if(BUILD_TESTING)
  add_subdirectory(autotests)
endif()

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
