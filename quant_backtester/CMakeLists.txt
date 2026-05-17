cmake_minimum_required(VERSION 3.16)
project(QuantBacktester VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(QUANT_ENABLE_PARQUET "Enable Parquet file ingestion support" OFF)
option(QUANT_BUILD_TESTS "Build test targets" ON)

add_subdirectory(src)

if(QUANT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/QuantBacktesterConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QuantBacktesterConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuantBacktester
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/QuantBacktesterConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QuantBacktesterConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/QuantBacktesterConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuantBacktester
)

set(CPACK_PACKAGE_NAME "QuantBacktester")
set(CPACK_PACKAGE_VENDOR "QuantBacktester")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "quant_backtester-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_GENERATOR "ZIP")
include(CPack)
