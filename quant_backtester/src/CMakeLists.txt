file(GLOB_RECURSE QUANT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/strategy/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/execution/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/portfolio/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/analytics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
)

add_library(quant_backtester_lib ${QUANT_SOURCES})

target_include_directories(quant_backtester_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(backtester ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(backtester PRIVATE quant_backtester_lib)
