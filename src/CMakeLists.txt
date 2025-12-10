add_executable(graceful main.c
        types.h
        global.h global.c

        common/log.h
        common/utils.h common/utils.c

        config/config.h config/config.c
)
target_include_directories(graceful PUBLIC ${XKB_INCLUDE_DIRS})
target_link_libraries(graceful PUBLIC ${XKB_LIBRARIES})