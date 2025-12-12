
set(ENV{PKG_CONFIG_PATH} /usr/local/lib64/pkgconfig:ENV{PKG_CONFIG_PATH})

set(WAYLAND_PROTOCOL_DIR /usr/share/wayland-protocols)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/stable/tablet/tablet-v2.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/tablet-v2-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/stable/tablet/tablet-v2.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/tablet-v2-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/stable/xdg-shell/xdg-shell.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/xdg-shell-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/stable/xdg-shell/xdg-shell.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/xdg-shell-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/staging/cursor-shape/cursor-shape-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/cursor-shape-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/staging/cursor-shape/cursor-shape-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/cursor-shape-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/staging/ext-workspace/ext-workspace-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-workspace-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/staging/ext-workspace/ext-workspace-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-workspace-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/staging/tearing-control/tearing-control-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/tearing-control-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/staging/tearing-control/tearing-control-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/tearing-control-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/staging/ext-image-copy-capture/ext-image-copy-capture-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-image-copy-capture-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/staging/ext-image-copy-capture/ext-image-copy-capture-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-image-copy-capture-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/staging/ext-image-capture-source/ext-image-capture-source-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-image-capture-source-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/staging/ext-image-capture-source/ext-image-capture-source-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-image-capture-source-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/pointer-constraints-unstable-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/pointer-constraints-unstable-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${WAYLAND_PROTOCOL_DIR}/staging/ext-foreign-toplevel-list/ext-foreign-toplevel-list-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-foreign-toplevel-list-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${WAYLAND_PROTOCOL_DIR}/staging/ext-foreign-toplevel-list/ext-foreign-toplevel-list-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/ext-foreign-toplevel-list-v1-protocol.c)


execute_process(COMMAND wayland-scanner client-header ${CMAKE_SOURCE_DIR}/src/protocols/wlr-layer-shell-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/wlr-layer-shell-unstable-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${CMAKE_SOURCE_DIR}/src/protocols/wlr-layer-shell-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/wlr-layer-shell-unstable-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${CMAKE_SOURCE_DIR}/src/protocols/wlr-output-power-management-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/wlr-output-power-management-unstable-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${CMAKE_SOURCE_DIR}/src/protocols/wlr-output-power-management-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/wlr-output-power-management-unstable-v1-protocol.c)

execute_process(COMMAND wayland-scanner client-header ${CMAKE_SOURCE_DIR}/src/protocols/wlr-foreign-toplevel-management-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/wlr-foreign-toplevel-management-unstable-v1-protocol.h)
execute_process(COMMAND wayland-scanner private-code  ${CMAKE_SOURCE_DIR}/src/protocols/wlr-foreign-toplevel-management-unstable-v1.xml
        ${CMAKE_SOURCE_DIR}/src/protocols/wlr-foreign-toplevel-management-unstable-v1-protocol.c)


