add_executable(nanners_test_virtual_uart
        test_virtual_uart.c
)

target_link_libraries(nanners_test_virtual_uart PRIVATE nanners)
target_include_directories(nanners_test_virtual_uart PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME nanners_virtual_uart COMMAND nanners_test_virtual_uart)

add_executable(nanners_test_crc_compute
        test_crc_compute.c
)

target_link_libraries(nanners_test_crc_compute PRIVATE nanners)
target_include_directories(nanners_test_crc_compute PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME nanners_crc_compute COMMAND nanners_test_crc_compute)