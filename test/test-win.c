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
#include <Windows.h>
#include "common-win.h"

int main(int argc, char *argv[])
{
    DWORD ids[4], results[4];
    HANDLE handles[4];
    handles[0] = CreateThread(0, 0, tst_tcp_client, NULL, 0, &ids[0]);
    handles[1] = CreateThread(0, 0, tst_tcp_server, NULL, 0, &ids[1]);
    handles[2] = CreateThread(0, 0, tst_udp_server, NULL, 0, &ids[2]);
    handles[3] = CreateThread(0, 0, tst_udp_client, NULL, 0, &ids[3]);
    for (int i = 0; i < 4; ++i)
        results[i] = WaitForSingleObject(handles[i], INFINITE);
    /* Collect test results */
    for (int i = 0; i < 4; ++i) {
        if (results[i] == WAIT_OBJECT_0) {
            // the thread handle is signaled - the thread has terminated
            DWORD exitcode;
            BOOL rc = GetExitCodeThread(handles[i], &exitcode);
            if (!rc) {
                printf("Unable to get thread status!\n");
                return -1;
            }
            if (exitcode != 0) {
                printf("Thread exited with status code %u!\n", exitcode);
                return exitcode;
            }
        }
        else {
            printf("Thread is still alive!");
        }
    }
    return 0;
}

