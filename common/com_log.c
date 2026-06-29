/**
 * @file com_log.c
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 日志模块
 * @version 1.0.0
 * @date 2026-06-27 07:51:45
 */

#include "com_log.h"
#include <stdarg.h>
#include <stddef.h>
<<<<<<< HEAD
#include <string.h>
=======
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
#include <stdio.h>

static log_level_t    g_log_level     = LOG_ALL;
static log_output_fn  g_log_output    = NULL;
static void          *g_log_user_data = NULL;

static const char *log_level_to_string(log_level_t level) {
    switch (level) {
    case LOG_ALL:   return "ALL";
    case LOG_ERROR: return "ERROR";
    case LOG_WARN:  return "WARN";
    case LOG_INFO:  return "INFO";
    case LOG_DEBUG: return "DEBUG";
    default:        return "UNKNOWN";
    }
}

static const char *log_extract_filename(const char *file)
{
<<<<<<< HEAD
    const char *p = strrchr(file, '/');
    const char *q = strrchr(file, '\\');

    if (q != NULL && (p == NULL || q > p)) {
        p = q;
    }

    return (p != NULL) ? (p + 1) : file;
=======
    const char *p = file;
    while (*p != '\0') {
        if (*p == '/' || *p == '\\') {
            p++;
        }
        p++;
    }
    /* 回退一步，指向最后一个分隔符之后的字符 */
    return (p > file) ? p - 1 : file;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
}

static void log_default_output(log_level_t level, const char *msg, void *user_data)
{
    (void)user_data;
<<<<<<< HEAD
    fprintf(stdout, "[%s]%s\n", log_level_to_string(level), msg);
=======
    fprintf(stdout, "[%s] %s\n", log_level_to_string(level), msg);
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
}

void log_set_level(log_level_t level) {
    g_log_level = level;
}

void log_set_output(log_output_fn fn, void *user_data) {
    g_log_output = fn;
    g_log_user_data = user_data;
}

void log_dispatch(log_level_t level, const char *file, int line, const char *fmt, ...)
{
    /* 级别过滤：只有 >= 当前阈值的日志才会输出 */
<<<<<<< HEAD
    if (level < g_log_level) {
=======
    if (level > g_log_level) {
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
        return;
    }

    char buf[1024];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    /* 截断时追加省略标记 */
    if (len < 0 || (size_t)len >= sizeof(buf)) {
        if ((size_t)len >= sizeof(buf)) {
            buf[sizeof(buf) - 4] = '.';
            buf[sizeof(buf) - 3] = '.';
            buf[sizeof(buf) - 2] = '.';
            buf[sizeof(buf) - 1] = '\0';
        }
    }

    char full_msg[2048];
    const char *fname = log_extract_filename(file);
    int full_len = snprintf(full_msg, sizeof(full_msg), "[%s:%d] %s", fname, line, buf);
    if (full_len < 0 || (size_t)full_len >= sizeof(full_msg)) {
        full_msg[sizeof(full_msg) - 4] = '.';
        full_msg[sizeof(full_msg) - 3] = '.';
        full_msg[sizeof(full_msg) - 2] = '.';
        full_msg[sizeof(full_msg) - 1] = '\0';
    }

    if (g_log_output != NULL) {
        g_log_output(level, full_msg, g_log_user_data);
    } else {
        log_default_output(level, full_msg, NULL);
    }
}