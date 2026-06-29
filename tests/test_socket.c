/**
 * @file test_posix_socket.c
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief 跨平台 Socket 库测试程序
 * @version 1.0.0
 * @date 2026-06-29 03:18:41
 * 
 * 测试 TCP/UDP 的服务器和客户端功能，验证跨平台 Socket 库的正确性。
 * 支持四种模式：TCP 服务器、TCP 客户端、UDP 服务器、UDP 客户端。
 *
 * 用法：
 *   ./test_socket <mode> <ip> [port]
 *
 * 模式：
 *   tcp_server  - TCP 服务器模式（监听并接受连接）
 *   tcp_client  - TCP 客户端模式（发起连接并发送数据）
 *   udp_server  - UDP 服务器模式（等待接收数据）
 *   udp_client  - UDP 客户端模式（发送数据并等待回复）
 *
 * 示例：
 *   @code
 *   # 终端1：启动 TCP 服务器
 *   ./test_socket tcp_server 0.0.0.0 2121
 *
 *   # 终端2：启动 TCP 客户端
 *   ./test_socket tcp_client 127.0.0.1 2121
 *   @endcode
 * 
 */

#include "com_log.h"
#include "pf_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief TCP 服务器测试函数
 *
 * 创建 TCP Socket，绑定地址，监听连接，接受一个客户端连接，接收数据后发送回显回复。
 * 
 * @param ipv4 监听 IP 地址，如 "0.0.0.0"
 * @param port 监听端口号
 */
static void test_tcp_server(const char *ipv4, uint16_t port) {
    LOG_INFO("Listenning address: %s:%u", ipv4, port);

    socket_t *listen_sock;
    socket_error_t err = socket_create_tcp(&listen_sock);
    if (SOCKET_OK != err) {
        LOG_ERROR("Create socket failed: %d", err);
        return;
    }

    inet_addr_t addr;
    err = socket_inet_aton(ipv4, port, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_inet_aton failed: %d", err);
        return;
    }

    err = socket_bind(listen_sock, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Bind failed: %d, last_error: %d", err, socket_get_error());
        socket_destroy(listen_sock);
    }

    err = socket_listen(listen_sock, 5);
    if (SOCKET_OK != err) {
        LOG_ERROR("Llistenind failed: %d", err, err);
        socket_destroy(listen_sock);
    }

    LOG_INFO("Listenning...");

    socket_t *client_sock;
    inet_addr_t client_addr;
    err = socket_accept(listen_sock, &client_sock, &client_addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Accept failed: %d", err);
        socket_destroy(listen_sock);
    }

    char addr_info[64];
    LOG_INFO("Client is connected: %s", 
        socket_inet_to_string(&client_addr, addr_info, sizeof(addr_info)));
    
    char recv_buf[1024];
    int n = socket_recv(client_sock, recv_buf, sizeof(recv_buf));
    if (n > 0) {
        recv_buf[n] = '\0';
        LOG_INFO("Received: %s", recv_buf);

        const char reply[] = "Hello from server!";
        socket_send(client_sock, reply, sizeof(reply) - 1);
        LOG_INFO("Reply: %s", reply);
    }

    socket_destroy(client_sock);
    socket_destroy(listen_sock);
}

/**
 * @brief TCP 客户端测试函数
 *
 * 创建 TCP Socket，连接到服务器，发送测试数据，接收服务器回显回复。
 * 
 * @param ipv4 服务器 IP 地址，如 "127.0.0.1"
 * @param port 服务器端口号
 */
static void test_tcp_client(const char *ipv4, uint16_t port) {
    LOG_INFO("Connect address: %s:%u", ipv4, port);

    socket_t *sock= NULL;
    socket_error_t err = socket_create_tcp(&sock);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_create_tcp failed: %d", err);
        return;
    }

    inet_addr_t addr;
    err = socket_inet_aton(ipv4, port, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_inet_aton failed: %d", err);
        return;
    }

    err = socket_connect(sock, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Connection failed: %d %d", err, socket_get_error());
        return;
    }

    LOG_INFO("Connected");

    const char msg[] = "Hello from client!";
    int n = socket_send(sock, msg, sizeof(msg) - 1);
    if (n > 0) {
        LOG_INFO("Sent: %s", msg);
    }

    char recv_buf[1024];
    n = socket_recv(sock, recv_buf, sizeof(recv_buf) - 1);
    if (n > 0) {
        recv_buf[n] = '\0';
        LOG_INFO("Received: %s", recv_buf);
    }

    socket_destroy(sock);
}

/**
 * @brief UDP 服务器测试函数
 *
 * 创建 UDP Socket，绑定地址，等待接收一个数据报，接收后发送回显回复。
 * 
 * @param ipv4 服务器 IP 地址，如 "127.0.0.1"
 * @param port 服务器端口号
 */
void test_udp_server(const char *ipv4, uint16_t port) {
    LOG_INFO("Listion: %s:%u", ipv4, port);

    socket_t *sock = NULL;
    socket_error_t err = socket_create_udp(&sock);
    if (SOCKET_OK != err) {
        LOG_ERROR("Create socket failed: %d", err);
        return;
    }

    inet_addr_t addr;
    err = socket_inet_aton(ipv4, port, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_inet_aton failed: %d", err);
        socket_destroy(sock);
        return;
    }

    err = socket_bind(sock, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Bind failed: %d", err);
        socket_destroy(sock);
        return;
    }

    LOG_INFO("Wait data...");

    char recv_buf[1024];
    inet_addr_t src_addr;
    int n = socket_recv_from(sock, recv_buf, sizeof(recv_buf) - 1, &src_addr);
    if (n > 0) {
        recv_buf[n] = '\0';
        char addr_info[64];
        LOG_INFO("Recv from %s, data: %s", 
            socket_inet_to_string(&src_addr, addr_info, sizeof(addr_info)),
            recv_buf);
        
        const char reply[] = "Pong from UDP server!";
        socket_send_to(sock, reply, sizeof(reply) - 1, &src_addr);
        LOG_INFO("Reply: %s", reply);
    }

    socket_destroy(sock);
}

/**
 * @brief UDP 客户端测试函数
 *
 * 创建 UDP Socket，向服务器发送一个数据报，等待接收服务器回显回复。
 * 客户端端口为服务器端口号+1。
 * 
 * @param ipv4 服务器 IP 地址，如 "127.0.0.1"
 * @param port 服务器端口号
 */
static void test_upd_client(const char *ipv4, uint16_t port) {
    LOG_INFO("Send address: %s:%d", ipv4, port);

    socket_t *sock;
    socket_error_t err = socket_create_udp(&sock);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_create_udp failed: %d", err);
        return;
    }

    /* 绑定本地地址，确保能收到对端回复 */
    inet_addr_t local_addr;
    err = socket_inet_aton("0.0.0.0", port+1, &local_addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_inet_aton failed: %d", err);
        socket_destroy(sock);
        return;
    }

    err = socket_bind(sock, &local_addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_bind failed: %d", err);
        socket_destroy(sock);
        return;
    }

    inet_addr_t addr;
    err = socket_inet_aton(ipv4, port, &addr);
    if (SOCKET_OK != err) {
        LOG_ERROR("Func socket_inet_aton failed: %d", err);
        socket_destroy(sock);
        return;
    }

    const char msg[] = "Ping from UDP client!";
    int n = socket_send_to(sock, msg, sizeof(msg) - 1, &addr);
    if (n > 0) {
        LOG_INFO("Send: %s", msg);
    }

    char recv_buf[1024];
    inet_addr_t src_addr;
    n = socket_recv_from(sock, recv_buf, sizeof(recv_buf) - 1, &src_addr);
    if (n > 0) {
        recv_buf[n] = '\0';
        char addr_buf[64];
        LOG_INFO("Received reply from %s: %s",
            socket_inet_to_string(&src_addr, addr_buf, sizeof(addr_buf)),
            recv_buf);
    } else {
        LOG_INFO("No reply received (timeout or error)");
    }

    socket_destroy(sock);
}

/**
 * @brief 测试程序入口
 *
 * 解析命令行参数，选择测试模式，执行对应测试。
 * 
 * @param argc 参数个数
 * @param argv 参数数组
 *   - argv[1]: 测试模式 (tcp_server/tcp_client/udp_server/udp_client)
 *   - argv[2]: IP 地址
 *   - argv[3]: 端口号（可选，默认 2121）
 * @return 0 成功，1 失败
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <mode> <ip> [port]\n", argv[0]);
        printf("\nMode:\n");
        printf("  tcp_server  - TCP Server Mode\n");
        printf("  tcp_client  - TCP Client Mode\n");
        printf("  udp_server  - UDP Server Mode\n");
        printf("  udp_client  - UDP Client Mode\n");
        printf("\nExample:\n");
        printf("  %s tcp_server 0.0.0.0 2121\n", argv[0]);
        printf("  %s tcp_client 127.0.0.1 2121\n", argv[0]);
        printf("  %s udp_server 0.0.0.0 2121\n", argv[0]);
        printf("  %s udp_client 127.0.0.1 2121\n", argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    const char *ip = argv[2];
    uint16_t port = (argc > 3) ? (uint16_t)atoi(argv[3]) : 2121;

    if (0 == strcmp(mode, "tcp_server")) {
        test_tcp_server(ip, port);
    } else if (0 == strcmp(mode, "tcp_client")) {
        test_tcp_client(ip, port);
    } else if (0 == strcmp(mode, "udp_server")) {
        test_udp_server(ip, port);
    } else if (0 == strcmp(mode, "udp_client")) {
        test_upd_client(ip, port);
    } else {
        LOG_ERROR("Unknown Mode: %s\n", mode);
    }

    return 0;
}