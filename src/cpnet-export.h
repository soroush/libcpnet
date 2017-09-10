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

#ifndef CPNET_EXPORT_H
#define CPNET_EXPORT_H

#ifdef _WIN32
#ifdef CPNET_NETWORK_EXPORT
#define CPNET_NETWORK_API __declspec(dllexport)
#else
#define CPNET_NETWORK_API __declspec(dllimport)
#endif
#else
#define CPNET_NETWORK_API
#endif

#endif
