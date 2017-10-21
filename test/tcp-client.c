/*
 * Copyright 2017 - FDE Co., <rabiei@fdeshragh.com>
 * This file is part of libfde_network.
 *
 * libfde_network is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libfde_network is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfde_network.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef __linux__
#include <unistd.h>
#endif // __linux__
#include "common.h"
#include "../src/cpnet-network.h"

#ifdef _WIN32
DWORD WINAPI tst_tcp_client(LPVOID param_arg)
#else
int main(int argc, char *argv[])
#endif
{
    srand((unsigned int)time(NULL));
    /* Initialize networking API */
    cpnet_init();
    /* Start a client */
    socket_t socket = cpnet_socket(SOCK_STREAM);
    size_t tries = 5;
    /* Connect to port TCP_PORT */
    while(cpnet_connect(socket, "127.0.0.1", TCP_PORT) != 0 && tries) {
#ifdef _WIN32
		Sleep(2000);
#else
		sleep(2);
#endif
        tries--;
        if(!tries){
            printf("Unable to connect!\n");
            return -1;
        }
    }
    /* Echo each message */
    size_t i = 0;
    for(i = 0; i < TEST_SIZE; ++i) {
        size_t len = (rand() % (1024 - 10)) + 10;
        /* char *str = randstring(len); */
        char *str = randdata(len);
        char buffer[1024];
        cpnet_write(socket, str, len);
        size_t read_size = cpnet_read(socket, buffer, 1024);
        if(read_size != len) {
            printf("Error: Expected to read %zd bytes, "
                   "though received %zd bytes.\n", len, read_size);
            return -1;
        }
        if(memcmp(str, buffer, len) != 0) {
            printf("Error: received data and sent data are not eaual.\n");
            return -1;
        }
#ifndef _WIN32
        if(i % (TEST_SIZE/1000) == 0)
            printf("Test: %07zd / %07d, (%0.2f%%)\n", i,
                   TEST_SIZE, (double)(i*100) / TEST_SIZE);
#endif
        free(str);
    }
    /* Finalize networking API */
    cpnet_clean();
    return 0;
}

