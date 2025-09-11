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
			"include/libecl/communication/i2c/bus_driver_interface.hpp"
			"include/libecl/communication/i2c/bus_scheduler.hpp"
			"include/libecl/communication/i2c/channel.hpp"
			"include/libecl/communication/i2c/transaction.hpp"
			"include/libecl/containers/aligned_storage.hpp"
			"include/libecl/containers/queue_spsc.hpp"
	PRIVATE
		"src/communication/i2c/bus_scheduler.cpp"
		"src/communication/i2c/channel.cpp"
)

target_compile_features(libecl
	PUBLIC
		cxx_std_20
)

if(PROJECT_IS_TOP_LEVEL)
	bob_configure_target(libecl)

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
