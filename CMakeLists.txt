cmake_minimum_required(VERSION 3.10)
project(graceful C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

add_definitions(
        -w -g
        -D PACKAGE_VERSION=\\"${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}\\"
)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package (PkgConfig)
pkg_check_modules(XKB xkbcommon xkbcommon-x11 REQUIRED)

add_subdirectory(src)