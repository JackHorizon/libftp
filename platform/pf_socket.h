/**
 * @file pf_socket.h
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 跨平台 Socket 封装库接口定义
 * @version 1.0.0
 * @date 2026-06-27 02:19:52
 * 
 * 提供统一的 TCP/UDP Socket API，屏蔽平台差异；支持 TCP 流式连接和 UDP 数据报通信。
 *
 * 使用示例：
 * @code {.c}
 *   socket_t *sock = NULL;
 *   socket_create_tcp(&sock);
 *
 *   inet_addr_t addr;
 *   socket_inet_aton("127.0.0.1", 8080, &addr); 
 *   socket_connect(sock, &addr);
 *
 *   socket_destroy(sock);
 * @endcode
 */

#ifndef PF_SOCKET_H
#define PF_SOCKET_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 跨平台 Socket 句柄
 *
 * 用户不应直接访问此结构体内部成员，仅通过指针使用 
 */
typedef struct socket socket_t;

/**
 * @brief IPv4 地址存储
 *
 * 地址字段和端口字段均以主机字节序存储，
 * 内部函数负责在系统调用时转换为网络字节序。
 */
typedef struct {
    uint32_t ipv4;      // IPv4 地址 (主机字节序)
    uint16_t port;      // 端口号 (主机字节序)
} inet_addr_t;

/**
 * @brief 平台相关 Socket 错误码
 */
typedef enum {
    SOCKET_OK             =  0,   // 操作成功
    SOCKET_ERROR          = -1,   // 通用错误
    SOCKET_ERR_NOMEM      = -2,   // 内存分配失败
    SOCKET_ERR_INVALID    = -3,   // 参数无效
#if _WIN32
    SOCKET_ERR_WSA        = -4,   // Windows WSA 错误
#endif
} socket_error_t;

/**
 * @brief socket_shutdown 的关闭方向常量
 */
enum {
    SOCKET_SHUT_RD      = 0,      // 禁止接收
    SOCKET_SHUT_WR      = 1,      // 禁止发送
    SOCKET_SHUT_RDWR    = 2,      // 禁止收发
};

/**
 * @brief 创建 TCP Socket
 *
 * 创建一个面向连接的 TCP Socket，使用 IPv4 协议族。
 *
 * @param[out] sock 输出参数，创建成功的 Socket 句柄
 * @return          SOCKET_OK 成功，SOCKET_ERR_NOMEM 内存不足，
 *                  SOCKET_ERR_INVALID 参数无效
 */
socket_error_t socket_create_tcp(socket_t **sock);

/**
 * @brief 创建 UDP Socket
 *
 * 创建一个无连接的 UDP Socket，使用 IPv4 协议族。
 *
 * @param[out] sock 输出参数，创建成功的 Socket 句柄
 * @return          SOCKET_OK 成功，SOCKET_ERR_NOMEM 内存不足，
 *                  SOCKET_ERR_INVALID 参数无效
 */
socket_error_t socket_create_udp(socket_t **sock);

/**
 * @brief 销毁 Socket 并释放资源
 *
 * 内部会自动关闭底层的 Socket 句柄并释放内存。
 * 调用后原指针无效，不应再使用该指针。
 *
 * @param sock  Socket 句柄（内部会 free，传入 NULL 安全）
 */
void socket_destroy(socket_t *sock);

/**
 * @brief 将 IP 字符串转换为 inet_addr_t
 *
 * 将点分十进制格式的 IP 字符串（如 "192.168.1.1"）转换为
 * inet_addr_t 结构体。
 *
 * @param[in]  ip   点分十进制 IP 字符串，如 "192.168.1.1"
 * @param[in]  port 端口号（主机字节序）
 * @param[out] addr 输出参数，转换后的地址结构
 * @return          SOCKET_OK 成功，SOCKET_ERR_INVALID 参数无效
 */
socket_error_t socket_inet_aton(const char *ip, uint16_t port, inet_addr_t *addr);

/**
 * @brief 绑定 Socket 到指定地址
 *
 * 内部自动设置 SO_REUSEADDR 选项，允许多个 Socket 绑定到同一端口。
 * 仅对 TCP 服务端或 UDP 服务端有意义。
 *
 * @param[in]  sock Socket 句柄
 * @param[in]  addr 本地地址（IP + 端口）
 * @return          SOCKET_OK 成功，SOCKET_ERR_INVALID 参数无效，
 *                  SOCKET_ERROR 绑定失败（端口被占用等）
 */
socket_error_t socket_bind(socket_t *sock, const inet_addr_t *addr);

/**
 * @brief 将 Socket 置于监听状态（仅 TCP）
 *
 * 仅对 TCP Socket 有效，UDP Socket 调用将返回 SOCKET_ERR_INVALID。
 *
 * @param[in] sock      Socket 句柄（TCP 类型）
 * @param[in] backlog   最大待连接队列长度
 * @return              SOCKET_OK 成功，SOCKET_ERR_INVALID 参数无效，
 *                      SOCKET_ERROR 监听失败
 */
socket_error_t socket_listen(socket_t *sock, int backlog);

/**
 * @brief 接受客户端连接（仅 TCP）
 *
 * 阻塞等待客户端连接，直到有连接到达或 Socket 被设置为非阻塞模式。
 *
 * @param[in]  sock        监听中的 Socket
 * @param[out] client      输出参数，新连接的 Socket 句柄（需单独销毁）
 * @param[out] client_addr 输出参数，客户端地址（可为 NULL，不获取地址时）
 * @return                 SOCKET_OK 成功，SOCKET_ERR_INVALID 参数无效，
 *                         SOCKET_ERROR 接受失败
 */
socket_error_t socket_accept(socket_t *sock, socket_t **client,
                                        inet_addr_t *client_addr);

/**
 * @brief 发起 TCP 连接
 *
 * 向远程服务器发起连接请求。若 Socket 未绑定，系统会自动分配本地地址。
 *
 * @param[in] sock   Socket 句柄（TCP 类型）
 * @param[in] addr   远程服务器地址
 * @return           SOCKET_OK 成功，SOCKET_ERR_INVALID 参数无效，
 *                   SOCKET_ERROR 连接失败（超时、拒绝等）
 */
socket_error_t socket_connect(socket_t *sock, const inet_addr_t *addr);

/**
 * @brief 发送数据（TCP）
 *
 * 发送指定长度的数据到对端。TCP 模式下数据以流的形式发送，
 * 不保证一次性发送完毕，调用方需检查返回值。
 *
 * @param[in] sock  Socket 句柄（TCP 类型）
 * @param[in] buf   数据缓冲区
 * @param[in] len   数据长度（字节）
 * @return          实际发送的字节数，SOCKET_ERROR 失败
 */
int socket_send(socket_t *sock, const void *buf, int len);

/**
 * @brief 接收数据（TCP）
 *
 * 从对端接收数据。TCP 模式下不保证一次性接收完所有数据，
 * 调用方需根据实际需求循环接收。
 *
 * @param[in]  sock  Socket 句柄（TCP 类型）
 * @param[out] buf   接收缓冲区
 * @param[in]  len   缓冲区最大长度
 * @return           实际接收的字节数，SOCKET_ERROR 失败（连接关闭时返回 0）
 */
int socket_recv(socket_t *sock, void *buf, int len);

/**
 * @brief 发送数据到指定地址（UDP）
 *
 * 向目标地址发送数据报。每次调用发送一个独立的数据报。
 *
 * @param[in] sock  Socket 句柄（UDP 类型）
 * @param[in] buf   数据缓冲区
 * @param[in] len   数据长度（字节）
 * @param[in] addr  目标地址
 * @return          实际发送的字节数，SOCKET_ERROR 失败
 */
int socket_send_to(socket_t *sock, const void *buf, int len,
                            const inet_addr_t *addr);

/**
 * @brief 从指定地址接收数据（UDP）
 *
 * 接收数据报并可选地获取发送方地址。
 *
 * @param[in]  sock      Socket 句柄（UDP 类型）
 * @param[out] buf       接收缓冲区
 * @param[in]  len       缓冲区最大长度
 * @param[out] src_addr  输出参数，发送方地址（可为 NULL，不获取地址时）
 * @return               实际接收的字节数，SOCKET_ERROR 失败
 */
int socket_recv_from(socket_t *sock, void *buf, int len,
                              inet_addr_t *src_addr);

/**
 * @brief 关闭 Socket 的读写通道
 *
 * 仅关闭通信通道，不释放 Socket 资源。调用 socket_destroy()
 * 才会真正释放 Socket。
 *
 * @param[in] sock  Socket 句柄
 * @param[in] how   关闭方向：SOCKET_SHUT_RD=禁止接收,
 *                  SOCKET_SHUT_WR=禁止发送, SOCKET_SHUT_RDWR=禁止收发
 */
void socket_shutdown(socket_t *sock, int how);

/**
 * @brief 设置非阻塞模式
 *
 * 设置为非阻塞模式后，接收/发送操作不会阻塞等待，
 * 无数据可收或缓冲区满时立即返回 SOCKET_ERROR。
 *
 * @param[in] sock      Socket 句柄
 * @param[in] non_block 1=非阻塞, 0=阻塞
 * @return              SOCKET_OK 成功，SOCKET_ERROR 设置失败
 */
socket_error_t socket_set_nonblock(socket_t *sock, int non_block);

/**
 * @brief 设置 TCP_NODELAY（禁用 Nagle 算法）
 *
 * 禁用 Nagle 算法后，数据会立即发送，减少延迟，
 * 但可能增加小数据包的数量。
 *
 * @param[in] sock Socket 句柄（TCP 类型）
 * @param[in] on   1=启用（禁用 Nagle）, 0=禁用（启用 Nagle）
 * @return         SOCKET_OK 成功，SOCKET_ERROR 设置失败
 */
socket_error_t socket_set_nodelay(socket_t *sock, int on);

/**
 * @brief 获取最后一次平台相关错误码
 *
 * Linux 下返回 errno。
 *
 * @return 平台相关的错误码
 */
int socket_get_error(void);

/**
 * @brief 将 inet_addr_t 转换为可读字符串
 *
 * 输出格式: "ip:port"，如 "192.168.1.1:21"。
 * 调用者需保证 buf 有足够的空间（建议至少 18 字节）。
 *
 * @param[in]  addr     地址结构
 * @param[out] buf      输出缓冲区
 * @param[in]  buf_size 缓冲区大小
 * @return              buf 指针（与传入的 buf 相同）
 */
const char *socket_inet_to_string(const inet_addr_t *addr, char *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* PF_SOCKET_H */