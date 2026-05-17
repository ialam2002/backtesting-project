#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QuantBacktester::quant_backtester_lib" for configuration "Release"
set_property(TARGET QuantBacktester::quant_backtester_lib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(QuantBacktester::quant_backtester_lib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/quant_backtester_lib.lib"
  )

list(APPEND _cmake_import_check_targets QuantBacktester::quant_backtester_lib )
list(APPEND _cmake_import_check_files_for_QuantBacktester::quant_backtester_lib "${_IMPORT_PREFIX}/lib/quant_backtester_lib.lib" )

# Import target "QuantBacktester::backtester" for configuration "Release"
set_property(TARGET QuantBacktester::backtester APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(QuantBacktester::backtester PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/backtester.exe"
  )

list(APPEND _cmake_import_check_targets QuantBacktester::backtester )
list(APPEND _cmake_import_check_files_for_QuantBacktester::backtester "${_IMPORT_PREFIX}/bin/backtester.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
