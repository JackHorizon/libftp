/**
 * @file socket.c
 * @author shenguoqing (sgq.work@foxmail.com)
 * @brief Socket 实现
 * @version 1.0.0
 * @date 2026-06-27 02:16:31
 * 
 * 基于 POSIX socket API 实现跨平台 Socket 接口。
 * 使用标准 socket()/bind()/listen()/accept()/send()/recv() 等系统调用。
 * 
 * 依赖：
 *   - Linux 平台无需额外链接库（socket 功能在 libc 中）
 *   - Windows 平台链接 ws2_32.lib
 */

#include "pf_mem.h"
#include "pf_socket.h"

#if _WIN32
#include <ws2tcpip.h>
#include <winsock2.h>
<<<<<<< HEAD
=======
#pragma comment(lib, "ws2_32.lib")
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif
#include <string.h>
#include <stdio.h>

struct socket {
    int handle; // POSIX 文件描述符
    int is_tcp; // 1#TCP, 0#UDP
};

socket_error_t socket_create_tcp(socket_t **sock) {
    if (!sock) return SOCKET_ERR_INVALID;

    socket_t *s = (socket_t *)pf_calloc(1, sizeof(socket_t));
    if (!s) return SOCKET_ERR_NOMEM;

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    s->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
    s->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
    if (s->handle < 0) {
        pf_free(s);
        return SOCKET_ERROR;
    }
    s->is_tcp = 1;
    *sock = s;
    return SOCKET_OK;
}

socket_error_t socket_create_udp(socket_t **sock) {
    if (!sock) return SOCKET_ERR_INVALID;

    socket_t *s = (socket_t *)pf_calloc(1, sizeof(socket_t));
    if (!s) return SOCKET_ERR_NOMEM;

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
<<<<<<< HEAD
    s->handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
    s->handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
=======
    s->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
#else
    s->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
#endif
    s->is_tcp = 0;
    *sock = s;
    return SOCKET_OK;
}

void socket_destroy(socket_t *sock) {
    if (!sock) return;

#ifdef _WIN32
    if (sock->handle != INVALID_SOCKET) closesocket(sock->handle);
#else
    if (sock->handle >= 0) close(sock->handle);
#endif
    pf_free(sock);
}

socket_error_t socket_inet_aton(const char *ip, uint16_t port, inet_addr_t *addr) {
    if (!ip || !addr) return SOCKET_ERR_INVALID;

    struct in_addr in_addr;
    if (inet_pton(AF_INET, ip, &in_addr) != 1) {
        return SOCKET_ERR_INVALID;
    }
<<<<<<< HEAD
    addr->ipv4 = ntohl(in_addr.s_addr);
=======
    addr->ipv4 = in_addr.s_addr;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
    addr->port = port;
    return SOCKET_OK;
}

socket_error_t socket_bind(socket_t *sock, const inet_addr_t *addr) {
    if (!sock || !addr) return SOCKET_ERR_INVALID;

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
<<<<<<< HEAD
    sa.sin_port = htons(addr->port);
    sa.sin_addr.s_addr = htonl(addr->ipv4);
=======
    sa.sin_port = addr->port;
    sa.sin_addr.s_addr = addr->ipv4;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517

    /* 允许 SO_REUSEADDR，避免端口被 TIME_WAIT 占用 */
    const int opt = 1;
    if (setsockopt(sock->handle, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        return SOCKET_ERROR;
    }

    if (bind(sock->handle, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        return SOCKET_ERROR;
    }
    return SOCKET_OK;
}

socket_error_t socket_listen(socket_t *sock, int backlog) {
    if (!sock) return SOCKET_ERR_INVALID;
    if (!sock->is_tcp) return SOCKET_ERR_INVALID;
    if (backlog < 1) backlog = 1;

    if (listen(sock->handle, backlog) < 0) {
        return SOCKET_ERROR;
    }
    return SOCKET_OK;
}

socket_error_t socket_accept(socket_t *sock, socket_t **client, inet_addr_t *client_addr) {
    if (!sock || !client) return SOCKET_ERR_INVALID;
    if (!sock->is_tcp) return SOCKET_ERR_INVALID;

    struct sockaddr_in peer = {0};
    socklen_t peer_len = sizeof(peer);
    int fd = accept(sock->handle, (struct sockaddr *)&peer, &peer_len);
    if (fd < 0) {
        return SOCKET_ERROR;
    }

    socket_t *s = (socket_t *)pf_calloc(1, sizeof(socket_t));
    if (!s) {
#ifdef _WIN32
        closesocket(fd);
#else
        close(fd);
#endif
        return SOCKET_ERR_NOMEM;
    }
    s->handle = fd;
    s->is_tcp = 1;

    *client = s;

    if (client_addr) {
<<<<<<< HEAD
        client_addr->ipv4 = ntohl(peer.sin_addr.s_addr);
        client_addr->port = ntohs(peer.sin_port);
=======
        client_addr->ipv4 = peer.sin_addr.s_addr;
        client_addr->port = peer.sin_port;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
    }
    return SOCKET_OK;
}

socket_error_t socket_connect(socket_t *sock, const inet_addr_t *addr) {
    if (!sock || !addr) return SOCKET_ERR_INVALID;
    if (!sock->is_tcp) return SOCKET_ERR_INVALID;

    struct sockaddr_in sa = {0};
    sa.sin_family      = AF_INET;
<<<<<<< HEAD
    sa.sin_port        = htons(addr->port);
    sa.sin_addr.s_addr = htonl(addr->ipv4);
=======
    sa.sin_port        = addr->port;
    sa.sin_addr.s_addr = addr->ipv4;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517

    if (connect(sock->handle, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        return SOCKET_ERROR;
    }
    return SOCKET_OK;
}

int socket_send(socket_t *sock, const void *buf, int len) {
    if (!sock || !buf || len <= 0) return SOCKET_ERROR;

    int sent = send(sock->handle, buf, len, 0);
    if (sent < 0) {
        return SOCKET_ERROR;
    }
    return sent;
}

int socket_recv(socket_t *sock, void *buf, int len) {
    if (!sock || !buf || len <= 0) return SOCKET_ERROR;

    int received = recv(sock->handle, buf, len, 0);
    if (received < 0) {
        return SOCKET_ERROR;
    }
    return received;
}

int socket_send_to(socket_t *sock, const void *buf, int len, const inet_addr_t *addr) {
    if (!sock || !buf || !addr || len <= 0) return SOCKET_ERROR;

    struct sockaddr_in sa = {0};
    sa.sin_family       = AF_INET;
<<<<<<< HEAD
    sa.sin_addr.s_addr  = htonl(addr->ipv4);
    sa.sin_port         = htons(addr->port);
=======
    sa.sin_addr.s_addr  = addr->ipv4;
    sa.sin_port         = addr->port;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517

    int sent = sendto(sock->handle, buf, len, 0, (struct sockaddr *)&sa, sizeof(sa));
    if (sent < 0) {
        return SOCKET_ERROR;
    }
    return sent;
}

int socket_recv_from(socket_t *sock, void *buf, int len, inet_addr_t *src_addr) {
    if (!sock || !buf || len <= 0) return SOCKET_ERROR;

    struct sockaddr_in peer = {0};
    socklen_t peer_len = sizeof(peer);

    int received = recvfrom(sock->handle, buf, len, 0,
                            (struct sockaddr *)&peer, &peer_len);
    if (received < 0) {
        return SOCKET_ERROR;
    }

    if (src_addr) {
<<<<<<< HEAD
        src_addr->ipv4 = ntohl(peer.sin_addr.s_addr);
        src_addr->port = ntohs(peer.sin_port);
=======
        src_addr->ipv4 = peer.sin_addr.s_addr;
        src_addr->port = peer.sin_port;
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
    }
    return received;
}

void socket_shutdown(socket_t *sock, int how) {
    if (!sock) return;

#ifdef _WIN32
    int how_win;
    switch (how) {
        case SOCKET_SHUT_RD: how_win = SD_RECEIVE; break;
        case SOCKET_SHUT_WR: how_win = SD_SEND; break;
        case SOCKET_SHUT_RDWR: how_win = SD_BOTH; break;
        default: return;
    }
    shutdown(sock->handle, how_win);
#else
    int how_unix;
    switch (how) {
        case SOCKET_SHUT_RD: how_unix = SHUT_RD; break;
        case SOCKET_SHUT_WR: how_unix = SHUT_WR; break;
        case SOCKET_SHUT_RDWR: how_unix = SHUT_RDWR; break;
        default: return;
    }
    shutdown(sock->handle, how_unix);
#endif
}

socket_error_t socket_set_nonblock(socket_t *sock, int non_block) {
    if (!sock) return SOCKET_ERR_INVALID;

#ifdef _WIN32
    u_long mode = (u_long)non_block;
    if (ioctlsocket(sock->handle, FIONBIO, &mode) != 0) {
        return SOCKET_ERR_WSA;
    }
#else
    int flags = fcntl(sock->handle, F_GETFL, 0);
    if (flags < 0) return SOCKET_ERROR;

    if (non_block) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (fcntl(sock->handle, F_SETFL, flags) < 0) {
        return SOCKET_ERROR;
    }
#endif
    return SOCKET_OK;
}

socket_error_t socket_set_nodelay(socket_t *sock, int on) {
    if (!sock) return SOCKET_ERR_INVALID;

    setsockopt(sock->handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(on));
    return SOCKET_OK;
}

int socket_get_error(void) {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

const char *socket_inet_to_string(const inet_addr_t *addr, char *buf, size_t buf_size) {
    if (!addr || !buf || buf_size == 0) return "";

    char ip_buf[16];
    (void)snprintf(ip_buf, sizeof(ip_buf), "%u.%u.%u.%u",
<<<<<<< HEAD
        (addr->ipv4 >> 24) & 0xFF,
        (addr->ipv4 >> 16) & 0xFF,
        (addr->ipv4 >> 8) & 0xFF,
        (addr->ipv4) & 0xFF
=======
        (addr->ipv4 << 24) & 0xFF,
        (addr->ipv4 << 16) & 0xFF,
        (addr->ipv4 << 8) & 0xFF,
        (addr->ipv4) & 0xff
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
    );

    (void)snprintf(buf, buf_size, "%s:%u", ip_buf, addr->port);
    return buf;
}