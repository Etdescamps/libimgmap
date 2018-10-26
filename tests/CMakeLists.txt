
add_executable(test_open test_open.c)
add_executable(test_create test_create.c)

target_link_libraries(test_open ${IMGMAP_LIB})
target_link_libraries(test_create ${IMGMAP_LIB})


