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

noinst_LTLIBRARIES = libfde_test_network_common.la
libfde_test_network_common_la_SOURCES = \
    common.c  \
    common.h

check_PROGRAMS = \
    client \
    server

client_SOURCES = \
    client.c

server_SOURCES = \
    server.c

LDADD = \
    ../src/libcpnet.la \
    libfde_test_network_common.la

TESTS = $(check_PROGRAMS)
