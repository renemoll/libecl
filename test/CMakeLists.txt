#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

find_package(Catch2 3 REQUIRED)
include(Catch)

add_executable(unittests)

target_sources(unittests
	PRIVATE
		test_queue_spsc.cpp
)

target_link_libraries(unittests
	PRIVATE
		libecl::libecl
		Catch2::Catch2WithMain
)

catch_discover_tests(unittests)
bob_generate_coverage_report(
	TARGET
		libecl-coverage
	RUNNER
		unittests
)

add_executable(unittests-tsan)

target_sources(unittests-tsan
	PRIVATE
		test_queue_spsc_tsan.cpp
)

target_link_libraries(unittests-tsan
	PRIVATE
		libecl::libecl
		Catch2::Catch2WithMain
)

catch_discover_tests(unittests-tsan)
bob_configure_target(unittests-tsan
	ENABLE_THREAD_SANITIZER On
	ENABLE_ADDRESS_SANITIZER Off
	ENABLE_LEAK_SANITIZER Off
)
