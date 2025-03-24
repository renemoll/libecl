#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

add_library(libecl)
add_library(libecl::libecl ALIAS libecl)

target_sources(libecl
	PUBLIC
		FILE_SET
			libecl_headers
		TYPE
			HEADERS
		BASE_DIRS
			"${CMAKE_CURRENT_LIST_DIR}/include"
		FILES
			"include/libecl/communication/bus_scheduler.h"
	PRIVATE
		"src/communication/bus_scheduler.cpp"
)

target_compile_features(libecl
	PUBLIC
		cxx_std_20
)

if(PROJECT_IS_TOP_LEVEL)
	bob_configure_project(libecl)

	install(
		TARGETS
			libecl
		EXPORT
			libeclTargets
		FILE_SET
			libecl_headers
	)

	install(
		EXPORT
			libeclTargets
		DESTINATION
			"share/cmake/libecl"
		NAMESPACE
			libecl::
	)

	install(
		FILES
			"${CMAKE_CURRENT_LIST_DIR}/cmake/libeclConfg.cmake"
		DESTINATION
			"share/cmake/libecl"
	)
endif()
