add_executable(checkerboard checkerboard.c)
target_link_libraries(checkerboard m runtime common)
target_include_directories(checkerboard PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_CURRENT_BINARY_DIR})

embed_file(string checkerboard_kernel_src checkerboard_kernel.slim)
