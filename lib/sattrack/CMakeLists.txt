set(SRCS
    sattrack.cc)

add_executable(sattrack
    ${SRCS})

# Add the include directory libsgp4
target_include_directories(sattrack PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsgp4)

target_link_libraries(sattrack
    sgp4)
