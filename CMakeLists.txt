cmake_minimum_required(VERSION 3.10)
project(graceful LANGUAGES C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(CMAKE_C_STANDARD_DEFAULT C99)

add_definitions(
        -w -g
        -D WLR_USE_UNSTABLE
        -D _POSIX_C_SOURCE=200809L
        -D PACKAGE_VERSION=\\"${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}\\"
)

include_directories(BEFORE
        /usr/local/include/
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/protocols
)

include(${CMAKE_SOURCE_DIR}/src/protocols/protocol.cmake)

find_package (PkgConfig)
pkg_check_modules(AQU aquamarine REQUIRED)
pkg_check_modules(PCRE libpcre2-8 REQUIRED)
pkg_check_modules(SCE scenefx-0.4 REQUIRED)
pkg_check_modules(WL wlroots-0.19 REQUIRED)
pkg_check_modules(XKB xkbcommon xkbcommon-x11 REQUIRED)
pkg_check_modules(WAYLAND wayland-server wayland-client REQUIRED)

add_subdirectory(src)