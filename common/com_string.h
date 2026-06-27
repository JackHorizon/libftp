/**
 * @file com_string.h
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 通用字符串操作
 * @version 1.0.0
 * @date 2026-06-27 08:11:11
 */

#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 安全的字符串拷贝，自动添加 null 终止符
 *
 * 从 src 拷贝最多 len 字节到 dst，但绝不会超出 dsizem 的边界。
 * 只要有空间，始终自动在末尾追加 '\0'。
 *
 * @param dst    目标缓冲区
 * @param dsizem 目标缓冲区大小（字节）
 * @param src    源字符串
 * @param len    最大拷贝字节数（不含 null 终止符）
 * @return size_t 实际拷贝到 dst 的字节数（不含 null 终止符）
 */
size_t com_strncpy(void *dst, size_t dsizem, const void *src, size_t len);

/**
 * @brief 安全的内存拷贝，不添加 null 终止符
 *
 * 从 src 拷贝最多 len 字节到 dst，但绝不会超出 dsizem 的边界。
 * 不自动追加 null 终止符，适用于任意二进制数据。
 *
 * @param dst    目标缓冲区
 * @param dsizem 目标缓冲区大小（字节）
 * @param src    源内存块
 * @param len    需要拷贝的字节数
 * @return size_t 实际拷贝的字节数
 */
size_t com_memncpy(void *dst, size_t dsizem, const void *src, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_STRING_H */
