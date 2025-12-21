cmake_minimum_required(VERSION 3.16)

project(nanners_consumer_test LANGUAGES C)

find_package(nanners CONFIG REQUIRED)
add_executable(consumer main.c)
target_link_libraries(consumer PRIVATE BANANUMS::nanners)
