include(ECMAddTests)
find_package(Qt5Test REQUIRED)
configure_file(config-kdesutest.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesutest.h)
ecm_add_test(kdesutest.cpp TEST_NAME kdesutest LINK_LIBRARIES Qt5::Test KF5::Su KF5::CoreAddons KF5::ConfigCore)

if(KDESU_USE_SUDO_DEFAULT)
  target_compile_definitions(kdesutest PRIVATE -DKDESU_USE_SUDO_DEFAULT="true")
elseif(KDESU_USE_DOAS_DEFAULT)
  target_compile_definitions(kdesutest PRIVATE -DKDESU_USE_DOAS_DEFAULT="true")
endif()
