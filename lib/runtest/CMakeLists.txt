set(SRCS
    runtest.cc)

add_executable(runtest
    ${SRCS})

# Add the include directory libsgp4
target_include_directories(runtest PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsgp4)

target_link_libraries(runtest
    sgp4)
