/**
 * @file com_log.h
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 通用日志模块
 * @version 1.0.0
 * @date 2026-06-27 08:11:41 
 */
 
#ifndef COM_LOG_H
#define COM_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_ALL = 0,  // 全部日志
    LOG_ERROR,    // 错误日志
    LOG_WARN,     // 警告日志
    LOG_INFO,     // 信息日志
    LOG_DEBUG,    // 调试日志
} log_level_t;

/**
 * @brief 日志输出回调函数类型
 *
 * @param level     日志级别
 * @param msg       日志消息内容
 * @param user_data 用户自定义数据（由 log_set_output 传入）
 */
typedef void (*log_output_fn)(log_level_t level, const char *msg, void *user_data);

/**
 * @brief 设置日志级别
 *
 * 只有级别 >= 当前设置的日志才会被输出。
 * 日志级别值越小，输出的日志越详细。
 *
 * @param level 新的日志级别
 */
void log_set_level(log_level_t level);

/**
 * @brief 设置日志输出回调函数
 *
 * 调用此函数后，所有日志消息将通过回调函数输出，
 * 而不是直接打印到 stdout。
 *
 * @param fn        日志输出回调函数，传 NULL 则恢复默认输出
 * @param user_data 传递给回调函数的用户自定义数据
 */
void log_set_output(log_output_fn fn, void *user_data);

/**
 * @brief 日志分发核心函数
 *
 * 由 LOG_* 宏调用，负责检查日志级别、格式化消息并输出。
 *
 * @param level 日志级别
 * @param file  源文件名（__FILE__）
 * @param line  源文件行号（__LINE__）
 * @param fmt   格式化字符串
 * @param ...   格式化参数
 */
void log_dispatch(log_level_t level, const char *file, int line, const char *fmt, ...);

/* 以下为日志宏，根据 LOG_LEVEL 决定是否实际输出 */
<<<<<<< HEAD
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_ALL
#endif
=======
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517

#if LOG_LEVEL <= LOG_DEBUG
#define LOG_DEBUG(fmt, ...) \
    log_dispatch(LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) \
    ((void)0)
#endif

#if LOG_LEVEL <= LOG_INFO
#define LOG_INFO(fmt, ...) \
    log_dispatch(LOG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) \
    ((void)0)
#endif

#if LOG_LEVEL <= LOG_WARN
#define LOG_WARN(fmt, ...) \
    log_dispatch(LOG_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...) \
    ((void)0)
#endif

#if LOG_LEVEL <= LOG_ERROR
#define LOG_ERROR(fmt, ...) \
    log_dispatch(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) \
    ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* COM_LOG_H */
