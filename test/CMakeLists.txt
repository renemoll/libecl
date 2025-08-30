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
		fakes/fake_bus_driver.cpp
		test_bus_scheduler.cpp
		test_queue.cpp
)

target_link_libraries(unittests
	PRIVATE
		libecl::libecl
		Catch2::Catch2WithMain
)

catch_discover_tests(unittests)
bob_create_coverage_report(
	NAME
		libecl-coverage
	RUNNER
		unittests
)
