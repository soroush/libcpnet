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

#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <time.h>
#include <Windows.h>
#endif
#include <stdlib.h>


#ifdef _WIN32
BOOLEAN nanosleep(LONGLONG ns)
{
    /* Declarations */
    HANDLE timer;   /* Timer handle */
    LARGE_INTEGER li;   /* Time defintion */
    /* Create timer */
    if(!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;
    /* Set timer properties */
    li.QuadPart = -ns;
    if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
        CloseHandle(timer);
        return FALSE;
    }
    /* Start & wait for timer */
    WaitForSingleObject(timer, INFINITE);
    /* Clean resources */
    CloseHandle(timer);
    /* Slept without problems */
    return TRUE;
}

LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;
    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime(int X, struct timeval *tv)
{
    LARGE_INTEGER t;
    FILETIME f;
    double microseconds;
    static LARGE_INTEGER offset;
    static double frequencyToMicroseconds;
    static int initialized = 0;
    static int usePerformanceCounter = 0;

    if(!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if(usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if(usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}
int CLOCK_REALTIME = 0;
#endif // _WIN32


static inline void get_hex(char *buf, int buf_len, char *hex_, int hex_len, int num_col)
{
    int i;
#define ONE_BYTE_HEX_STRING_SIZE   3
    unsigned int byte_no = 0;
    if(buf_len <= 0) {
        if(hex_len > 0) {
            hex_[0] = '\0';
        }
        return;
    }
    if(hex_len < ONE_BYTE_HEX_STRING_SIZE + 1) {
        return;
    }
    do {
        for(i = 0; ((i < num_col) && (buf_len > 0) && (hex_len > 0)); ++i) {
            snprintf(hex_, hex_len, "%02X ", buf[byte_no++] & 0xff);
            hex_ += ONE_BYTE_HEX_STRING_SIZE;
            hex_len -= ONE_BYTE_HEX_STRING_SIZE;
            buf_len--;
        }
        if(buf_len > 1) {
            snprintf(hex_, hex_len, "\n");
            hex_ += 1;
        }
    } while((buf_len) > 0 && (hex_len > 0));
}

long nanodiff(struct timespec *a, struct timespec *b)
{
    //printf(" A: %010ld %010ld\n", a->tv_sec, a->tv_nsec);
    //printf(" B: %010ld %010ld\n", then->tv_sec, then->tv_nsec);
    long sdiff = (a->tv_sec - b->tv_sec) * 1.0E+09;
    return sdiff + (a->tv_nsec - b->tv_nsec);
}

void sleep_for(struct timespec *then, long sleep_value)
{
    struct timespec now;
    if(clock_gettime(CLOCK_REALTIME, &now) == -1) {
        fprintf(stderr, "Unable to get system clock\n%s\n", strerror(errno));
        exit(99);
    }
    //printf("  NOW: %010ld %010ld\n", now.tv_sec, now.tv_nsec);
    //printf(" THEN: %010ld %010ld\n", then->tv_sec, then->tv_nsec);
    long diff = nanodiff(&now, then);
    //printf("last packet diff: %ld\n", diff);
    if(diff < sleep_value) {
        const long sleep_nanoseconds = sleep_value - diff;
        struct timespec sleep_time;
        sleep_time.tv_sec = sleep_nanoseconds * (long)1.0E-09;
        sleep_time.tv_nsec = sleep_nanoseconds % (long)1.0E+09;
        //printf("SLEEP: %010ld %010ld (s)\n", sleep_time.tv_sec, sleep_time.tv_nsec / 1000000);
#ifdef _WIN32
        nanosleep(sleep_nanoseconds);
#else
        nanosleep(&sleep_time, NULL);
#endif // _WIN32
    }
}

char *randstring(size_t length)
{

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *random_string = NULL;
    size_t n = 0;
    if(length) {
        random_string = malloc(sizeof(char) * (length + 1));
        if(random_string) {
            for(n = 0; n < length; n++) {
                int key = rand() % (int)(sizeof(charset) - 1);
                random_string[n] = charset[key];
            }

            random_string[length] = '\0';
        }
    }

    return random_string;
}

char *randdata(size_t length)
{
    char *random_data = NULL;
    size_t n = 0;
    if(length) {
        random_data = malloc(sizeof(char) * (length));
        if(random_data) {
            for(n = 0; n < length; ++n) {
                random_data[n] = rand() % 0xFF;
            }
        }
    }
    return random_data;
}
