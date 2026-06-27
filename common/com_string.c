/**
 * @file com_string.c
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 
 * @version 1.0.0
 * @date 2026-06-27 06:35:39
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "com_string.h"

size_t com_strncpy(void *dst, size_t dsizem, const void *src, size_t len) {
    if (dst == NULL || src == NULL || dsizem == 0) {
        return 0;
    }

    size_t copy_len = (len < dsizem - 1) ? len : (dsizem - 1);

    for (size_t i = 0; i < copy_len; i++) {
        ((char *)dst)[i] = ((const char *)src)[i];
    }
    ((char *)dst)[copy_len] = '\0';

    return copy_len;
}

size_t com_memncpy(void *dst, size_t dsizem, const void *src, size_t len) {
    if (dst == NULL || src == NULL || dsizem == 0) {
        return 0;
    }

    size_t copy_len = (len < dsizem) ? len : dsizem;

    for (size_t i = 0; i < copy_len; i++) {
        ((char *)dst)[i] = ((const char *)src)[i];
    }

    return copy_len;
}

