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

AM_CXXFLAGS = --pedantic -Wall -Werror

lib_LTLIBRARIES = libcpnet.la
libcpnet_la_LDFLAGS = -version-info $(CPNET_SO_VERSION)
libcpnet_la_SOURCES = \
    cpnet-network.c

libfde_logger_includedir = $(includedir)/cpnet
libfde_logger_include_HEADERS =  \
    cpnet-network.h \
    cpnet-export.h
