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

AC_PREREQ([2.69])
AC_INIT([libcpnet], [1.0.0])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
LT_INIT([static shared pic-only])

# Configure automake
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 libcpnet.pc])

AC_SUBST([CPNET_SO_VERSION], [1:0:0])
AC_SUBST([CPNET_API_VERSION], [1.0.0])

AC_OUTPUT
