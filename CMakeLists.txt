cmake_minimum_required(VERSION 3.27)
project(satellite-coverage-tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the sgp4 library
add_subdirectory(lib/libsgp4)

# Add the passpredict library
add_subdirectory(lib/passpredict)

# Add the runtest library
add_subdirectory(lib/runtest)

# Add the sattrack library
add_subdirectory(lib/sattrack)

# Add the executable
add_executable(main src/main.cpp)

# Link the libsgp4 library to the executable
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsgp4)

target_link_libraries(main
        sgp4)