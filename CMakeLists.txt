cmake_minimum_required(VERSION 3.27)
project(satellite-coverage-tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Catch2 3 REQUIRED)

# Add the sgp4 library
add_subdirectory(lib/libsgp4)

# Add the passpredict library
add_subdirectory(lib/passpredict)

# Add the runtest library
add_subdirectory(lib/runtest)

# Add the sattrack library
add_subdirectory(lib/sattrack)

# Add the main executable
add_executable(main src/main.cpp
        src/TLEParser.cpp
        src/TLEParser.h
        src/SatelliteCoverageTracker.cpp
        src/SatelliteCoverageTracker.h
        src/GraphModel.cpp
        src/GraphModel.h
        src/LinearModel.cpp
        src/LinearModel.h
        src/SatelliteCatalog.cpp
        src/SatelliteCatalog.h
        src/Utility.cpp
        src/Utility.h
        src/DataModel.cpp
        src/DataModel.h)

# Link the libsgp4 library to the main executable
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsgp4)
target_link_libraries(main PRIVATE sgp4)

# Add the tests executable
add_executable(tests tests/libsgp4-tests.cpp
        tests/TLEParser-tests.cpp
        tests/Graph-tests.cpp
        tests/LinearModel-tests.cpp
        tests/SatelliteCatalog-tests.cpp
        src/TLEParser.cpp
        src/TLEParser.h
        src/SatelliteCoverageTracker.cpp
        src/SatelliteCoverageTracker.h
        src/GraphModel.cpp
        src/GraphModel.h
        src/LinearModel.cpp
        src/LinearModel.h
        src/SatelliteCatalog.cpp
        src/SatelliteCatalog.h
        src/Utility.cpp
        src/Utility.h
        tests/Utility-tests.cpp
        src/DataModel.cpp
        src/DataModel.h
        tests/DataModel-tests.cpp)

# Link the libsgp4 library to the tests executable
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsgp4)
target_link_libraries(tests PRIVATE sgp4 Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
