/**
 * @file posix_mem.c
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief POSIX 平台内存管理实现
 * @version 1.0.0
 * @date 2026-06-27
 */

#include "pf_mem.h"
#include <stdlib.h>

void *pf_malloc(size_t size) {
    return malloc(size);
}

void *pf_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

void *pf_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

void pf_free(void *ptr) {
    free(ptr);
}

void pf_safe_free(void **ptr) {
    if (*ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}