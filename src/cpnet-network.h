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

/**
 * \brief Initialize networking interface.
 */
CPNET_NETWORK_API int cpnet_init();

/**
 * \brief Sets option on socket. Only options that could be treated as boolean
 * can be set by this function. For other usages see \ref net_setval .
 */
CPNET_NETWORK_API int cpnet_setopt(socket_t s, int option);

/**
 * \brief Unsets option on socket. Only options that could be treated as boolean
 * can be set by this function. For other usages see \ref net_setval .
 */
CPNET_NETWORK_API int cpnet_unsetopt(socket_t s, int option);

/**
 * \brief Sets options with numeral values on sockest. Only options with
 * arguments of integer semantics should be set by this function. For setting
 * and unsetting boolean options please refere to \ref net_setopt and
 * \ref net_unsetopt .
 */
CPNET_NETWORK_API int cpnet_setval(socket_t s, int option, int val);

/**
 * \brief Creates a socket instance.
 */
CPNET_NETWORK_API socket_t cpnet_socket(int type);

/**
 * \brief Binds a socket on specified address and port.
 */
CPNET_NETWORK_API int cpnet_bind(socket_t s, const char *address, uint16_t *port);

/**
 * \brief Connects the socket referred to by the file descriptor, to the address
 * specified by IPv4 address and port number
 */
CPNET_NETWORK_API int cpnet_connect(socket_t s, const char *address, uint16_t port);

/**
 * \brief Marks  the  socket  as a passive socket, that is, as a socket that
 * will be used to accept incoming connection requests.
 */
CPNET_NETWORK_API int cpnet_listen(socket_t s, int backlog);

/**
  * The accept() function is used with connection-based socket types
  * (SOCK_STREAM). This function extracts the first connection request on the queue
  * of pending connections for the listening socket, `s', creates a new connected
  * socket, and returns a new handler referring to that socket.
  * The newly created socket is not in the listening state. The original socket
  * `s' is unaffected by this call.
  * The argument `s' is a socket that has been created with \ref cpnet_socket,
  * bound to a local address with \ref cpnet_bind, and is listening for connections
  * after a \ref cpnet_listen.
  */
CPNET_NETWORK_API socket_t cpnet_accept(socket_t s, char *addr, uint16_t *port);
CPNET_NETWORK_API ssize_t cpnet_read(socket_t s, char *buffer, size_t len);
CPNET_NETWORK_API ssize_t cpnet_read2(socket_t s, char *buffer, size_t len, int flags);
CPNET_NETWORK_API ssize_t cpnet_write(socket_t s, const char *buffer, size_t len);
CPNET_NETWORK_API ssize_t cpnet_read_packet(socket_t s, char *buffer, size_t len,
        char *peer, uint16_t *port);
CPNET_NETWORK_API ssize_t cpnet_read_packet_s(socket_t s, char *buffer, size_t len,
        struct sockaddr *cl_addr);
CPNET_NETWORK_API ssize_t cpnet_write_packet(socket_t s, char *buffer, size_t len,
        const char *addrest, uint16_t port);
CPNET_NETWORK_API ssize_t cpnet_write_packet_s(socket_t s, char *buffer, size_t len,
        const struct sockaddr_in *cl_addr);
CPNET_NETWORK_API struct sockaddr_in *cpnet_inet_addr(const char *address,
        uint16_t port);
CPNET_NETWORK_API void cpnet_close(socket_t s);
CPNET_NETWORK_API int cpnet_clean();
CPNET_NETWORK_API const char *cpnet_last_error();


#ifdef __cplusplus
}
#endif

#endif /* CPNET_NETWOEK_H */
