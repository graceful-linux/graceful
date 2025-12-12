//
// Created by dingjing on 2025/12/10.
//

#ifndef graceful_GRACEFUL_CONFIG_H
#define graceful_GRACEFUL_CONFIG_H
#include <ctype.h>
#include <libgen.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "types.h"


// 整数版本 - 截断小数部分
#define CLAMP_INT(_x, _min, _max)   (((int) (_x) < (int) (_min)) ? (int) (_min) : (((int) (_x) > (int) (_max)) ? (int) (_max) : (int) (_x)))
#define CLAMP_FLOAT(_x, _min, _max) (((_x) < (_min)) ? (_min) : ((_x) > (_max)) ? (_max) : (_x))






void config_reload      (void);
void config_parse       (void);
void config_override    (void);
void config_free        (void);
void config_parse_file  (Config* config, const char* filePath);


#endif // graceful_GRACEFUL_CONFIG_H
