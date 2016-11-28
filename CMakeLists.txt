# Copyright (c) 1996-2016, OPC Foundation. All rights reserved.
#
# The source code in this file is covered under a dual-license scenario:
#   - RCL: for OPC Foundation members in good-standing
#   - GPL V2: everybody else
#
# RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/
#
# GNU General Public License as published by the Free Software Foundation;
# version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2
#
# This source code is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cmake_minimum_required(VERSION 2.8.11)

project(uastack C)

option(UASTACK_GENERATE_COVERAGE "Build the project with coverage output" OFF)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /wd4255 /wd4820")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -W -Wall")

    if (UASTACK_GENERATE_COVERAGE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    endif ()
endif ()

find_package(OpenSSL REQUIRED)

add_subdirectory(Stack)
add_subdirectory(AnsiCSample)
