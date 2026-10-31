cmake_minimum_required(VERSION 3.13...3.31)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(IOBU_BUILD_EXAMPLES "Build IOBU examples" OFF)

if(IOBU_BUILD_EXAMPLES)
    list(APPEND VCPKG_MANIFEST_FEATURES "examples")
endif()

project(
  iobu
  VERSION 1.0.0
  LANGUAGES CXX)

file(GLOB_RECURSE PROJECT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*cpp)
add_library(${PROJECT_NAME} ${PROJECT_SRCS})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_definitions(${PROJECT_NAME} INTERFACE)

if(IOBU_BUILD_EXAMPLES)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/IobuExamples.cmake)
endif()

# installation section

include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.hh"
    PATTERN "*.inl")

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION ${CONFIG_INSTALL_DIR})
