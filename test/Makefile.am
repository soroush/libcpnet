# Copyright 2017 - Soroush Rabiei <soroush@ametisco.ir>
# This file is part of libcpnet.
#
# libcpnet is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libcpnet is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libcpnet.  If not, see <http://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -I$(srcdir)/..
AM_CXXFLAGS = --pedantic -Wall -Werror

noinst_LTLIBRARIES = libcpnet_test.la
libcpnet_test_la_SOURCES = \
    common.c  \
    common.h

check_PROGRAMS = \
    tcp_client \
    tcp_server \
    udp_client \
    udp_server

tcp_client_SOURCES = \
    tcp-client.c

tcp_server_SOURCES = \
    tcp-server.c

udp_client_SOURCES = \
    udp-client.c

udp_server_SOURCES = \
    udp-server.c

LDADD = \
    ../src/libcpnet.la \
    libcpnet_test.la

TESTS = $(check_PROGRAMS)
