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

add_library(QuantBacktester::quant_backtester_lib ALIAS quant_backtester_lib)

target_include_directories(quant_backtester_lib
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/third_party
)

# Conditionally enable Parquet support
if(QUANT_ENABLE_PARQUET)
    find_package(Arrow REQUIRED)
    find_package(Parquet REQUIRED)
    
    target_link_libraries(quant_backtester_lib PRIVATE Arrow::arrow_shared Parquet::parquet_shared)
    target_compile_definitions(quant_backtester_lib PRIVATE QUANT_PARQUET_ENABLED)
endif()

add_executable(backtester ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(backtester PRIVATE quant_backtester_lib)

install(TARGETS quant_backtester_lib backtester
    EXPORT QuantBacktesterTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT QuantBacktesterTargets
    NAMESPACE QuantBacktester::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuantBacktester
)
