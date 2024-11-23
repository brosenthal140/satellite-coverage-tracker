set(SRCS
    passpredict.cc)

add_executable(passpredict
    ${SRCS})

# Add the include directory libsgp4
target_include_directories(passpredict PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsgp4)

target_link_libraries(passpredict
    sgp4)
