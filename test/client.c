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

int main(int argc, char *argv[])
{
    srand(time(NULL));
    /* Initialize networking API */
    net_init();
    /* Start a client */
    socket_t socket = net_socket(SOCK_STREAM);
    /* Connect to port 50001 */
    net_connect(socket, "127.0.0.1", 50001);
    /* Echo each message */
    size_t i = 0;
    for(i = 0; i < 1000000; ++i) {
        int len = (rand() % (1024 - 10)) + 10;
        /* char *str = randstring(len); */
        char *str = randdata(len);
        char buffer[1024];
        net_write(socket, str, len);
        /*
        printf("Iter: %d\n", i);
        printf("Size: %d\n", len);
        printf("WRITE\n");
        */
        int read_size = net_read(socket, buffer, 1024);
        if(read_size != len) {
            printf("Error: Expected to read %d bytes, though received %d bytes.\n", len, read_size);
            exit(-1);
        }
        if(memcmp(str, buffer, len) != 0) {
            printf("Error: received data and sent data are not eaual.\n");
            exit(-1);
        }
        if(i % 5000 == 0)
            printf("Test: %07l / %07l, (%0.2f%%)\n", i, 1000000, (float)i / 10000);
        /*
        str[len]='\0';
        printf("Size: %d\n", len);
        printf("Sent:\n%s\n", str);
        usleep(100);
        */
        free(str);
    }
    /* Finalize networking API */
    net_clean();
    exit(0);
}

