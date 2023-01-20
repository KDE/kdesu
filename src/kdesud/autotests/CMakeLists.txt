include(ECMAddTests)
find_package(Qt${QT_MAJOR_VERSION}Test REQUIRED)
configure_file(config-kdesudtest.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdesudtest.h)
ecm_add_test(kdesudtest.cpp ../lexer.cpp TEST_NAME kdesudtest LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test KF6::CoreAddons KF6::ConfigCore)
