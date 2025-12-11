cmake_minimum_required(VERSION 3.10)
project(graceful C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

add_definitions(
        -w -g
        -D WLR_USE_UNSTABLE
        -D PACKAGE_VERSION=\\"${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}\\"
)

set(ENV{PKG_CONFIG_PATH} /usr/local/lib64/pkgconfig:ENV{PKG_CONFIG_PATH})

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package (PkgConfig)
pkg_check_modules(PCRE libpcre2-8 REQUIRED)
pkg_check_modules(AQU aquamarine REQUIRED)
pkg_check_modules(SCE scenefx-0.4 REQUIRED)
pkg_check_modules(WL wlroots-0.19 REQUIRED)
pkg_check_modules(XKB xkbcommon xkbcommon-x11 REQUIRED)

add_subdirectory(src)