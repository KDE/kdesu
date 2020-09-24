include(ECMAddTests)
find_package(Qt5Test REQUIRED)
configure_file(config-kdesudtest.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesudtest.h)
ecm_add_test(kdesudtest.cpp ../lexer.cpp TEST_NAME kdesudtest LINK_LIBRARIES Qt5::Test KF5::CoreAddons KF5::ConfigCore)
