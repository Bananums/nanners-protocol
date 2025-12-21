cmake_minimum_required(VERSION 3.16)

project(nanners-protocol VERSION 0.1.0 LANGUAGES C)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTING "Enable tests" OFF)

include(GNUInstallDirs)

set(NANNERS_NAME nanners)

option(NANNERS_DEBUG "Enable Nanners debug logging" OFF)

add_library(${NANNERS_NAME} src/nanners.c src/putters.c src/logs.c)
add_library(BANANUMS::${NANNERS_NAME} ALIAS ${NANNERS_NAME})  # build-tree alias

set_target_properties( ${NANNERS_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_features(nanners PUBLIC c_std_11)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/nanners/version.h
        @ONLY
)

target_include_directories(${NANNERS_NAME}
        PUBLIC
          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
          $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (NANNERS_DEBUG)
    target_compile_definitions(nanners PRIVATE NANNERS_DEBUG=ON)
endif()

# Enable CTest & test subtree
if(BUILD_TESTS)
    include(CTest)            # defines BUILD_TESTING option
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


# Install headers and library
install(TARGETS nanners
        EXPORT nannersTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/include/nanners/version.h
        DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/nanners
)


# Package exports (BANANUMS::nanners for consumers)
install(EXPORT nannersTargets
        NAMESPACE BANANUMS::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nanners
        FILE nannersTargets.cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/nannersConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMinorVersion
)
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nannersConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/nannersConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nanners
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/nannersConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/nannersConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nanners
)

# Export for build-tree usage (optional quality-of-life)
export(EXPORT nannersTargets
        NAMESPACE BANANUMS::
        FILE "${CMAKE_CURRENT_BINARY_DIR}/nannersTargets.cmake")