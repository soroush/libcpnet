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

#include "cpnet-network.h"
#include <stdlib.h>
#include <string.h>
#if defined(__linux__)
#include <netdb.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#elif defined(_WIN32)
#include <WS2tcpip.h>
#include <Windef.h>
#include <stdio.h>
#include <io.h>
#endif

#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif

static void net_set_last_error();

/**
  * Calling this function on POSIX platforms (or wherever BSD sockets are
  * supported) has no effect. On Windows platforms, this function initializes
  * Winsock API by calling \ref WSAStartup.
  */

CPNET_NETWORK_API
int net_init()
{
#ifdef _WIN32
    WORD requested_version;
    WSADATA wsa_data;
    int err;
    requested_version = MAKEWORD(2, 2);
    err = WSAStartup(requested_version, &wsa_data);
    if(err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return -1;
    }
    /* Confirm that the WinSock DLL supports 2.2.        */
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if(LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return -1;
    } else
        return 0;
#endif
}

static inline int setsockopt_int_helper(socket_t s, int option, int value)
{
    switch(option) {
    case SO_BROADCAST:
    case SO_DEBUG:
    case SO_DONTROUTE:
    case SO_KEEPALIVE:
    case SO_OOBINLINE:
    case SO_REUSEADDR:
#ifndef _WIN32
    /* Windows only knows the SO_REUSEADDR option, there is no SO_REUSEPORT.
     * Setting SO_REUSEADDR on a socket in Windows behaves like setting
     * SO_REUSEPORT and SO_REUSEADDR on a socket in BSD, with one
     * exception: A socket with SO_REUSEADDR can always bind to exactly the
     * same source address and port as an already bound socket, even if the
     * other socket did not have this option set when it was bound.
     */
    case SO_REUSEPORT:
#endif
    case SO_SNDBUF:
    case SO_RCVBUF:
    case SO_SNDLOWAT:
    case SO_RCVLOWAT:
#ifdef SO_USELOOPBACK
    case SO_USELOOPBACK:
#endif
        return setsockopt(s, SOL_SOCKET, option,
                          (const void *)(&value), sizeof(value));
        break;
    case SO_SNDTIMEO:
    case SO_RCVTIMEO: {
        /* Windows and Linux timeout values are different. Windows expects an
         * int which is number of milliseconds to wait, linux expects a struct
         * of timeval type. This function expects microseconds.
         */
#if defined(_WIN32)
        DWORD sock_timeout = value / 1000;
#else
        const struct timeval sock_timeout = {.tv_sec = value / 1000000,
                                             .tv_usec = value % 1000000};
#endif
        return setsockopt(s, SOL_SOCKET, option,
                          (const void *)(&sock_timeout), sizeof(sock_timeout));
    }
    break;
    default:
        return -1;
        break;
    }
}

/**
  * Supported options are:
  * * SO_BROADCAST
  * * SO_DEBUG
  * * SO_DONTROUTE
  * * SO_KEEPALIVE
  * * SO_OOBINLINE
  * * SO_REUSEADDR
  *
  * NOTE: calling this function with other types of options, will cause a logical
  * error and will behave differently on different platforms.
  */
CPNET_NETWORK_API
int net_setopt(socket_t s, int option)
{
    int ret = setsockopt_int_helper(s, option, 1);
    if(ret != 0)
        net_set_last_error();
    return ret;
}

/**
  * Supported options are:
  * * SO_BROADCAST
  * * SO_DEBUG
  * * SO_DONTROUTE
  * * SO_KEEPALIVE
  * * SO_OOBINLINE
  * * SO_REUSEADDR
  *
  * NOTE: calling this function with other types of options, will cause a logical
  * error and will behave differently on different platforms.
  */
CPNET_NETWORK_API
int net_unsetopt(socket_t s, int option)
{
    int ret = setsockopt_int_helper(s, option, 0);
    if(ret != 0)
        net_set_last_error();
    return ret;
}

CPNET_NETWORK_API
int net_setval(socket_t s, int option, int val)
{
    int ret = setsockopt_int_helper(s, option, val);
    if(ret != 0)
        net_set_last_error();
    return ret;
}

CPNET_NETWORK_API
socket_t net_socket(int type)
{
    return socket(AF_INET, type, 0);
}

CPNET_NETWORK_API
int net_bind(socket_t sockfd, const char *address, uint16_t *portno)
{
    if(!sockfd)
        return -1;
    struct sockaddr_in serv_addr;
    memset((void *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if(address)
#if defined (__linux__)
        inet_aton(address, &serv_addr.sin_addr);
#elif defined(_WIN32)
        inet_pton(AF_INET, address, &serv_addr.sin_addr);
#endif
    else
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(*portno);
    int status_code = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(status_code == 0 && *portno == 0) {
        struct sockaddr addr;
        socklen_t addrlen = sizeof(addr);
        if(getsockname(sockfd, &addr, &addrlen) == 0) {
            struct sockaddr_in *sin = (struct sockaddr_in *)(&addr);
            *portno = ntohs(sin->sin_port);
        }
    }
    /*
    Since SOCKET_ERROR is defined as (-1) in WinSock API, it is safe to assume
    retval < 0 results in an error; unless SOCKET_ERROR has changed to some positive
    value, which is unlikely.
    */
    if(status_code < 0) {
        net_set_last_error();
    }
    return status_code;
}

CPNET_NETWORK_API
int net_connect(socket_t sockfd, const char *address, uint16_t portno)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    server = gethostbyname(address);
    if(server == NULL) {
        return -1;
    }
    memset((void *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    return connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

CPNET_NETWORK_API
int net_listen(socket_t socketfd, int backlog)
{
    return listen(socketfd, backlog);
}

CPNET_NETWORK_API
socket_t net_accept(socket_t socketfd, char *addr, uint16_t *port)
{
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    socket_t rvalue = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);
    if(rvalue > 0) {
        *port = ntohs(cli_addr.sin_port);
#if defined (__linux__)
        char *tmp = inet_ntoa(cli_addr.sin_addr);
        strcpy(addr, tmp);
#elif defined(_WIN32)
        char buffer[46];
        inet_ntop(AF_INET, &cli_addr.sin_addr, buffer, 46);
        strcpy(addr, buffer);
#endif
    }
    return rvalue;
}

CPNET_NETWORK_API
ssize_t net_read(socket_t socketfd, char *buffer, size_t len)
{
#if defined (__linux__)
    ssize_t retval = recv(socketfd, buffer, len, MSG_NOSIGNAL);
#elif defined(_WIN32)
    ssize_t retval = (ssize_t)(recv(socketfd, buffer, (int)len, 0));
#endif
    if(retval <= 0)
        net_set_last_error();
    return retval;
}


CPNET_NETWORK_API
ssize_t net_read_packet(socket_t socketfd, char *buffer,
                        size_t len, char *peer_address, uint16_t *peer_port)
{
    ssize_t recv_size = 0;
    struct sockaddr peer;
    socklen_t peer_len = sizeof(struct sockaddr);
    struct sockaddr_in *peer_addr;
#if defined (__linux__)
    recv_size = recvfrom(socketfd, buffer, len, MSG_NOSIGNAL, &peer, &peer_len);
#elif defined(_WIN32)
    recv_size = (ssize_t)recvfrom(socketfd, buffer, (int)len, 0, &peer, &peer_len);
#endif
    if(peer.sa_family == AF_INET) {
        peer_addr = (struct sockaddr_in *)(&peer);
    }
    if(peer_address) {
        strcpy(peer_address, inet_ntoa(peer_addr->sin_addr));
    }
    if(peer_port) {
        *peer_port = ntohs(peer_addr->sin_port);
    }
    return recv_size;
}

CPNET_NETWORK_API
ssize_t net_write_packet(socket_t socketfd, char *buffer, size_t len,
                         const char *address, uint16_t port)
{
    ssize_t send_size;
    struct sockaddr_in *cl_addr = net_inet_addr(address, port);
    send_size = net_write_packet_s(socketfd, buffer, len, cl_addr);
    free(cl_addr);
    return send_size;
}

CPNET_NETWORK_API
ssize_t net_write_packet_s(socket_t socketfd, char *buffer, size_t len,
                           const struct sockaddr_in *dst)
{
    ssize_t send_size;
#if defined (__linux__)
    send_size = sendto(socketfd, buffer, len, MSG_NOSIGNAL,
                       (const struct sockaddr *) dst, sizeof(struct sockaddr_in));
#elif defined(_WIN32)
    send_size = sendto(socketfd, buffer, (int)len, 0,
                       (const struct sockaddr *) dst, sizeof(struct sockaddr_in));
#endif
    if(send_size != len) {
        net_set_last_error();
    }
    return send_size;
}

CPNET_NETWORK_API
struct sockaddr_in *net_inet_addr(const char *address, uint16_t port)
{
    struct sockaddr_in *cl_addr = (struct sockaddr_in *)(malloc(sizeof(struct sockaddr_in)));
    memset((void *)cl_addr, '\0', sizeof(struct sockaddr_in));
    cl_addr->sin_family = AF_INET;
    if(!address) {
        cl_addr->sin_addr.s_addr = htonl(INADDR_BROADCAST);
    } else {
        cl_addr->sin_addr.s_addr = inet_addr(address);
    }
    cl_addr->sin_port = htons(port);
    return cl_addr;
}


CPNET_NETWORK_API
ssize_t net_write(socket_t socketfd, const char *buffer, size_t len)
{
    if(socketfd < 0)
        return -1;
#if defined (__linux__)
    return send(socketfd, buffer, len, MSG_NOSIGNAL);
#elif defined(_WIN32)
    return send(socketfd, buffer, (unsigned int)len, 0);
#endif
}

CPNET_NETWORK_API int net_clean()
{
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif /* _WIN32 */
}

CPNET_NETWORK_API
const char *net_last_error()
{
    return cpnet_last_error;
}

static void net_set_last_error()
{
#if defined(_WIN32)
    /* FIXME: Error message is not working in windows*/
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, WSAGetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR)cpnet_last_error, 0, NULL);
#elif defined(__linux__)
    strcpy(cpnet_last_error, strerror(errno));
#endif
}

CPNET_NETWORK_API
void net_close(socket_t socketfd)
{
#if defined(_WIN32)
    closesocket(socketfd);
#elif defined(__linux__)
    close(socketfd);
#endif
}

