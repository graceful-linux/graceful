//
// Created by dingjing on 2025/12/10.
//

#include "utils.h"

#include <ctype.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "log.h"


void utils_trim_whitespace(char* str)
{
    if (str == NULL || *str == '\0') {
        return;
    }

    // Trim leading space
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // Trim trailing space
    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    // Move the trimmed part to the beginning if needed
    if (start != str) {
        memmove(str, start, end - start + 2); // +2 to include null terminator
    }
}

char* utils_sanitize_string(char* str)
{
    while (*str != '\0' && !isprint((unsigned char)*str)) {
        str++;
    }
    char *end = str + strlen(str) - 1;
    while (end > str && !isprint((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

bool utils_regex_match(const char* pattern, const char* str)
{
    int errNum = 0;
    PCRE2_SIZE errOffset = 0;

    if (!pattern || !str) {
        return false;
    }

    pcre2_code *re = pcre2_compile((PCRE2_SPTR)pattern, PCRE2_ZERO_TERMINATED, PCRE2_UTF, &errNum, &errOffset, NULL);
    if (!re) {
        PCRE2_UCHAR errBuf[256] = {0};
        pcre2_get_error_message(errNum, errBuf, sizeof(errBuf));
        DWARN("PCRE2 error: %s at offset %zu\n", (const char*) errBuf, errOffset);
        return false;
    }

    pcre2_match_data *matchData = pcre2_match_data_create_from_pattern(re, NULL);
    const int ret = pcre2_match(re, (PCRE2_SPTR)str, strlen(str), 0, 0, matchData, NULL);

    pcre2_match_data_free(matchData);
    pcre2_code_free(re);

    return ret >= 0;
}
