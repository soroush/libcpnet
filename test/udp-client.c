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
DWORD WINAPI tst_udp_client(LPVOID param_arg)
#else
int main(int argc, char *argv[])
#endif
{
    /* Give time to server to bind (or else will be deadlock) */
#ifdef _WIN32
	Sleep(2000);
#else
	sleep(2);
#endif
    srand((unsigned int)time(NULL));
    /* Initialize networking API */
    cpnet_init();
    /* Start a client */
    socket_t socket = cpnet_socket(SOCK_DGRAM);
    /* Echo each message */
    size_t i = 0;
    size_t fails = 0;
    for(i = 0; i < TEST_SIZE; ++i) {
        int len = (rand() % (1024 - 10)) + 10;
        /* char *str = randstring(len); */
        char *str = randdata(len);
        char buffer[1024];
        cpnet_write_packet(socket, str, len, "127.0.0.1", UDP_PORT);
        ssize_t read_size = cpnet_read_packet(socket, buffer, 1024, NULL, NULL);
        if(read_size != len) {
#ifdef _WIN32
            printf("Error: Expected to read %d bytes, though received %lld bytes.\n", len, read_size);
#else
            printf("Error: Expected to read %d bytes, though received %ld bytes.\n", len, read_size);
#endif
            fails++;
            free(str);
            continue;
        }
        if(memcmp(str, buffer, len) != 0) {
            printf("Error: received data and sent data are not eaual.\n");
            fails++;
            free(str);
            continue;
        }
#ifndef _WIN32
        if(i % (TEST_SIZE / 1000) == 0)
            printf("Test: %07zd / %07d, (%04.2f%%)\n", i,
                   TEST_SIZE, (double)(i * 100) / TEST_SIZE);
#endif
        free(str);
    }
    /* Finalize networking API */
    cpnet_clean();
    double fr = (double)(fails) / TEST_SIZE;
    if(fr > EXPECTED_UDP_FAILURE) {
        printf("Failure rate (%04.2f%%) is bigger than expected (%04.2f%%)\n",
               fr, EXPECTED_UDP_FAILURE);
    }
    return 0;
}

