//
// Created by dingjing on 2025/12/10.
//

#ifndef graceful_GRACEFUL_GLOBAL_H
#define graceful_GRACEFUL_GLOBAL_H
#include "types.h"

#define GRACEFUL_SYS_CONF_DIR               "/etc"

extern int              gLogLevel;
extern Config           gConfig;
extern const char*      gConfigFilePath;

extern GlobalConfig     gGlobalConfig;          // 全局配置

void init_global_config (void);



#endif // graceful_GRACEFUL_GLOBAL_H
