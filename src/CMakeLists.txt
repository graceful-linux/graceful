file(GLOB protocolSrc
        ${CMAKE_SOURCE_DIR}/src/protocols/*.h
        ${CMAKE_SOURCE_DIR}/src/protocols/*.c
)

add_executable(graceful main.c
        ${protocolSrc}
        types.h keymap.h
        global.h global.c

        common/log.h
        common/utils.h common/utils.c

        wm/wm.h wm/wm.c
        wm/wayland.h wm/wayland.c

        config/config.h config/config.c
#        ext-protocol/wlr_ext_workspace_v1.h ext-protocol/wlr_ext_workspace_v1.c
)
message(STATUS ${protocolSrc})

target_link_libraries(graceful PUBLIC
        ${WL_LIBRARIES}
        ${XKB_LIBRARIES}
        ${AQU_LIBRARIES}
        ${SCE_LIBRARIES}
        ${PCRE_LIBRARIES}
        ${WAYLAND_LIBRARIES}
)

target_include_directories(graceful PUBLIC
        ${WL_INCLUDE_DIRS}
        ${XKB_INCLUDE_DIRS}
        ${AQU_INCLUDE_DIRS}
        ${SCE_INCLUDE_DIRS}
        ${PCRE_INCLUDE_DIRS}
        ${WAYLAND_INCLUDE_DIRS}
)
