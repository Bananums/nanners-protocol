
add_executable(get_frame get_frame.c)
target_link_libraries(get_frame BANANUMS::nanners)

add_executable(send_frame send_frame.c)
target_link_libraries(send_frame BANANUMS::nanners)
