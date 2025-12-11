//
// Created by dingjing on 2025/12/10.
//

#ifndef graceful_GRACEFUL_UTILS_H
#define graceful_GRACEFUL_UTILS_H
#include <string.h>
#include <stdbool.h>

void    utils_trim_whitespace   (char* str);

/**
 * @brief 清除字符中不可见字符
 * @param str
 * @note 没有分配内存
 * @return
 */
char*   utils_sanitize_string   (char* str);

bool    utils_regex_match       (const char *patternMb, const char *strMb);

#endif // graceful_GRACEFUL_UTILS_H
