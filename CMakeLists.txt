#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/bob-cmake")

project(libecl
	LANGUAGES
		CXX
	VERSION
		0.1.0
)

if(PROJECT_IS_TOP_LEVEL)
	include(bob)
endif()

add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory(benchmark)

	enable_testing()
	add_subdirectory(test)
endif()
