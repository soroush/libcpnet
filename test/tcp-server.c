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

#include <stdlib.h>
#include <stdio.h>
#ifdef __linux__
#include <unistd.h>
#endif /* __linux__ */
#include "common.h"
#include "../src/cpnet-network.h"

#ifdef _WIN32
DWORD WINAPI tst_tcp_server(LPVOID param_arg)
#else
int main(int argc, char *argv[])
#endif
{
    /* Initialize networking API (if any needed) */
    net_init();
    /* Start a server */
    socket_t socket = net_socket(SOCK_STREAM);
    /* Bind on port TCP_PORT */
    uint16_t port = TCP_PORT;
    if(net_bind(socket, NULL, &port) != 0) {
        printf("System error description:\n%s\n", net_last_error());
        return -1;
    }
    /* Listen for incomming connections */
    net_listen(socket, 10);
    /* Accept clients */
    char address[46];
    socket_t client = net_accept(socket, address, &port);
    printf("Client connected. Remote Address:`%s' "
           "Assigned port number: %d\n", address, port);
    /* Echo each message */
    size_t i;
    for(i = 0; i < TEST_SIZE; ++i) {
        char buffer[1024];
        ssize_t r = net_read(client, buffer, 1024);
        if(r < 0) {
            //printf(strerror("Unable to read from socket!"));
            return -1;
        }
        if(r == 0) {
            printf("Remote client stopped\n");
            exit(-1);
        }
        net_write(client, buffer, r);
    }
    return 0;
}

