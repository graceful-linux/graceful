add_executable(graceful main.c
        types.h keymap.h
        global.h global.c

        common/log.h
        common/utils.h common/utils.c

#        config/config.h config/config.c
)
target_link_libraries(graceful PUBLIC
        ${XKB_LIBRARIES} ${AQU_LIBRARIES} ${SCE_LIBRARIES} ${WL_LIBRARIES} ${PCRE_LIBRARIES})
target_include_directories(graceful PUBLIC ${XKB_INCLUDE_DIRS} ${AQU_INCLUDE_DIRS} ${SCE_INCLUDE_DIRS} ${WL_INCLUDE_DIRS} ${PCRE_INCLUDE_DIRS})
