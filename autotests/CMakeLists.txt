include(ECMAddTests)
find_package(Qt5Test REQUIRED)
configure_file(config-kdesutest.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesutest.h)
ecm_add_test(kdesutest.cpp TEST_NAME kdesutest LINK_LIBRARIES Qt5::Test KF5::Su KF5::CoreAddons KF5::ConfigCore)
