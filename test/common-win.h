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

#ifndef CPNET_NETWORK_TEST_COMMON_WIN_H
#define CPNET_NETWORK_TEST_COMMON_WIN_H

#define _WINSOCKAPI_  /* stops windows.h including winsock.h */
#include <Windows.h>

DWORD WINAPI tst_tcp_client(LPVOID);
DWORD WINAPI tst_tcp_server(LPVOID);
DWORD WINAPI tst_udp_client(LPVOID);
DWORD WINAPI tst_udp_server(LPVOID);

#endif  // CPNET_NETWORK_TEST_COMMON_WIN_H
