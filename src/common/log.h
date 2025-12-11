//
// Created by dingjing on 2025/12/10.
//

#ifndef graceful_GRACEFUL_LOG_H
#define graceful_GRACEFUL_LOG_H
#include "types.h"
#include "global.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DDIE(fmt, ...)      do { if (gLogLevel >= GRACEFUL_LOG_LEVEL_DIE)     { fprintf(stderr, "[%s %s:%d] [DIE]   " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); } exit(1); } while (0)
#define DERROR(fmt, ...)    do { if (gLogLevel >= GRACEFUL_LOG_LEVEL_ERROR)   { fprintf(stderr, "[%s %s:%d] [ERR]   " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); } exit(1); } while (0)
#define DWARN(fmt, ...)     do { if (gLogLevel >= GRACEFUL_LOG_LEVEL_WARNING) { fprintf(stderr, "[%s %s:%d] [WARN]  " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); } exit(1); } while (0)
#define DINFO(fmt, ...)     do { if (gLogLevel >= GRACEFUL_LOG_LEVEL_INFO)    { fprintf(stderr, "[%s %s:%d] [INFO]  " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); } exit(1); } while (0)
#define DDEBUG(fmt, ...)    do { if (gLogLevel >= GRACEFUL_LOG_LEVEL_DEBUG)   { fprintf(stderr, "[%s %s:%d] [DEBUG] " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); } exit(1); } while (0)




#endif // graceful_GRACEFUL_LOG_H
