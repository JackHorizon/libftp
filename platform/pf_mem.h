/**
 * @file pf_mem.h
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 跨平台内存管理
 * @version 1.0.0
 * @date 2026-06-27 03:17:05
 */

#ifndef PF_MEM_H
#define PF_MEM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 分配指定大小的内存块
 *
 * 等价于标准库 malloc()。分配成功返回指向已分配内存的指针，
 * 失败返回 NULL。返回的内存内容未初始化。
 *
 * @param[in] size 要分配的字节数
 * @return 成功时返回指向已分配内存的指针，失败返回 NULL
 */
void *pf_malloc(size_t size);

/**
 * @brief 分配并清零内存块
 *
 * 等价于标准库 calloc()。分配 nmemb × size 字节的内存，
 * 并将所有字节初始化为 0。
 *
 * @param[in] nmemb 元素个数
 * @param[in] size  每个元素的字节数
 * @return 成功时返回指向已分配内存的指针，失败返回 NULL
 */
void *pf_calloc(size_t nmemb, size_t size);

/**
 * @brief 重新分配内存块
 *
 * 等价于标准库 realloc()。将内存块的大小调整为 size 字节。
 * 如果 ptr 为 NULL，行为等价于 pf_malloc(size)；
 * 如果 size 为 0 且 ptr 非 NULL，行为等价于 pf_free(ptr)。
 *
 * @param[in] ptr  原内存块指针
 * @param[in] size 新大小（字节）
 * @return 成功时返回指向新内存块的指针（可能与 ptr 不同），失败返回 NULL
 */
void *pf_realloc(void *ptr, size_t size);

/**
 * @brief 释放内存
 *
 * 等价于标准库 free()。释放由 pf_malloc/pf_calloc/pf_realloc 分配的内存。
 * 传入 NULL 是安全的，不会产生任何效果。
 *
 * @param ptr 要释放的内存指针（内部会 free，传入 NULL 安全）
 */
void pf_free(void *ptr);

/**
 * @brief 安全释放内存并将指针置为 NULL
 *
 * 先释放 ptr 指向的内存，然后将 *ptr 置为 NULL，
 * 避免悬空指针导致二次释放或野指针访问。
 *
 * @param ptr 指向指针的指针（内部会 free 并置 NULL，传入 NULL 安全）
 */
void pf_safe_free(void **ptr);

#ifdef __cplusplus
}
#endif

#endif /* PF_MEM_H */