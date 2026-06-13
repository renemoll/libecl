#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

cmake_minimum_required(VERSION 3.23)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/bob-cmake")

project(libecl
	LANGUAGES
		CXX
	VERSION
		0.1.0
)

#
# Project options.
#

set(LIBECL_DEFAULT_OPTION ${PROJECT_IS_TOP_LEVEL})
option(LIBECL_BUILD_BENCHMARKS "Build benchmarks" ${LIBECL_DEFAULT_OPTION})
option(LIBECL_BUILD_TESTS "Build tests" ${LIBECL_DEFAULT_OPTION})


#
# C++ standard configuration.
# Allow for C++ standard to be set via CLI.
#

if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

#
# Include cmake utilities, bob.
#

include(bob)

#
# Add project code.
#

add_subdirectory(libecl)

if(LIBECL_BUILD_BENCHMARKS AND NOT CMAKE_CROSSCOMPILING)
	add_subdirectory(benchmark)
endif()

if(LIBECL_BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)
	enable_testing()
	add_subdirectory(test)
endif()
