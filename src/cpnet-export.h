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

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
  #ifdef libcpnet_EXPORTS
    #ifdef __GNUC__
      #define CPNET_NETWORK_API __attribute__ ((dllexport))
    #else
      #define CPNET_NETWORK_API __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define CPNET_NETWORK_API __attribute__ ((dllimport))
    #else
      #define CPNET_NETWORK_API __declspec(dllimport)
    #endif
  #endif
  #define CPNET_NETWORK_PRIVATE
#else
  #if __GNUC__ >= 4
    #define CPNET_NETWORK_API __attribute__ ((visibility ("default")))
    #define CPNET_NETWORK_PRIVATE  __attribute__ ((visibility ("hidden")))
  #else
    #define CPNET_NETWORK_API
    #define CPNET_NETWORK_PRIVATE
  #endif
#endif

#endif