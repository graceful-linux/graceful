//
// Created by dingjing on 2025/12/12.
//

#include "wm.h"

#include "global.h"
#include "wayland.h"


void wm_log_init(void)
{
    switch (gLogLevel) {
        default:
        case GRACEFUL_LOG_LEVEL_DEBUG: {
            wayland_set_log_level(WLR_DEBUG);
            break;
        }
        case GRACEFUL_LOG_LEVEL_NONE: {
            wayland_set_log_level(WLR_SILENT);
            break;
        }
        case GRACEFUL_LOG_LEVEL_INFO: {
            wayland_set_log_level(WLR_INFO);
            break;
        }
        case GRACEFUL_LOG_LEVEL_WARNING: {
            wayland_set_log_level(WLR_LOG_IMPORTANCE_LAST);
            break;
        }
        case GRACEFUL_LOG_LEVEL_ERROR: {
            wayland_set_log_level(WLR_ERROR);
            break;
        }
    }
}

void wm_quit(void)
{
    // wayland
    wayland_quit();
}

bool wm_display_init(void)
{
    return wayland_display_init();
}
