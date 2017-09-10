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

#ifndef CPNET_NETWORK_TEST_COMMON_H
#define CPNET_NETWORK_TEST_COMMON_H

#include <errno.h>
#include <time.h>

/* Prototypes for common functions */

static inline void get_hex(char *buf, int buf_len, char *hex_, int hex_len, int num_col);
long nanodiff(struct timespec *a, struct timespec *b);
void sleep_for(struct timespec *then, long nanoseconds);
char *randstring(size_t length);
char *randdata(size_t length);

#endif  // CPNET_NETWORK_TEST_COMMON_H
