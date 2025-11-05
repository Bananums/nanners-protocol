cmake_minimum_required(VERSION 3.16)

project(nanners-protocol LANGUAGES C)

set(NANNERS_NAME nanners)

add_library(${NANNERS_NAME} src/nanners.c)

target_include_directories(${NANNERS_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable CTest & test subtree
SET(BUILD_TESTING ON)
if(BUILD_TESTING)
    include(CTest)            # defines BUILD_TESTING option
    add_subdirectory(tests)
endif()