//
// Created by dingjing on 2025/12/12.
//

#ifndef graceful_GRACEFUL_WAYLAND_H
#define graceful_GRACEFUL_WAYLAND_H
#include "wlr/util/log.h"



void    wayland_set_log_level   (enum wlr_log_importance level);
void    wayland_quit            (void);
bool    wayland_display_init    (void);


#endif // graceful_GRACEFUL_WAYLAND_H
