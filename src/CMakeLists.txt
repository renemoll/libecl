#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

add_library(libecl
	STATIC
		"${CMAKE_CURRENT_LIST_DIR}/communication/bus_scheduler.cpp"
)
target_include_directories(libecl
	PUBLIC
		"${PROJECT_SOURCE_DIR}/include"
)

if(PROJECT_IS_TOP_LEVEL)
	bob_configure_project(libecl)
endif()
