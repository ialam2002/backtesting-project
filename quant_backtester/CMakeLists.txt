cmake_minimum_required(VERSION 3.16)
project(QuantBacktester VERSION 0.1.0 LANGUAGES CXX)

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
