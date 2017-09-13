/*
 * Copyright 2017 - Soroush Rabiei <soroush@ametisco.ir>
 * This file is part of libcpnet.
 *
 * libcpnet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcpnet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libcpnet.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPNET_NETWOEK_H
#define CPNET_NETWOEK_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "cpnet-export.h"
#ifdef _WIN32
#include <basetsd.h>
#include <WinSock2.h>
#endif // _WIN32
#if defined(__linux__)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#endif // __linux__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__)
typedef int socket_t;
#elif defined(_WIN32)
typedef SOCKET socket_t;
typedef SSIZE_T ssize_t;
#endif

static char cpnet_last_error[1024];

/**
 * \brief Initialize networking interface.
 */
CPNET_NETWORK_API int net_init();

/**
 * \brief Sets option on socket. Only options that could be treated as boolean
 * can be set by this function. For other usages see \ref net_setval .
 */
CPNET_NETWORK_API int net_setopt(socket_t s, int option);
/**
 * \brief Unsets option on socket. Only options that could be treated as boolean
 * can be set by this function. For other usages see \ref net_setval .
 */
CPNET_NETWORK_API int net_unsetopt(socket_t s, int option);
CPNET_NETWORK_API int net_setval(socket_t s, int option, int val);
CPNET_NETWORK_API socket_t net_socket(int type);
CPNET_NETWORK_API int net_bind(socket_t s, const char *address, uint16_t *port);
CPNET_NETWORK_API int net_connect(socket_t s, const char *address, uint16_t port);
CPNET_NETWORK_API int net_listen(socket_t s, int backlog);
CPNET_NETWORK_API socket_t net_accept(socket_t s, char *addr, uint16_t *port);
CPNET_NETWORK_API ssize_t net_read(socket_t s, char *buffer, size_t len);
CPNET_NETWORK_API ssize_t net_write(socket_t s, const char *buffer, size_t len);
CPNET_NETWORK_API ssize_t net_read_packet(socket_t s, char *buffer, size_t len,
        char *peer, uint16_t *port);
CPNET_NETWORK_API ssize_t net_write_packet(socket_t s, char *buffer, size_t len,
        const char *addrest, uint16_t port);
CPNET_NETWORK_API ssize_t net_write_packet_s(socket_t s, char *buffer, size_t len,
        const struct sockaddr_in *cl_addr);
CPNET_NETWORK_API struct sockaddr_in *net_inet_addr(const char *address,
        uint16_t port);
CPNET_NETWORK_API void net_close(socket_t s);
CPNET_NETWORK_API int net_clean();
CPNET_NETWORK_API const char *net_last_error();


#ifdef __cplusplus
}
#endif

#endif /* CPNET_NETWOEK_H */
