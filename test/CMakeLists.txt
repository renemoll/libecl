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
		test_aligned_storage.cpp
		test_bus_scheduler.cpp
		test_queue_spsc.cpp
		test_byte_reader.cpp
		test_byte_writer.cpp
)

target_link_libraries(unittests
	PRIVATE
		libecl::libecl
		Catch2::Catch2WithMain
)

catch_discover_tests(unittests)
bob_configure_target(unittests
    # TODO: either use scan-build or ensure system headers use -isystem to avoid warnings
	ENABLE_CLANG_TIDY Off
)
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
	ENABLE_CLANG_TIDY Off
	#
	# The idea was this test should run with ThreadSanitizer, however there is some kind of  issue
	# with it. There are some workarounds regarding the address space layout randomization (ASLR),
	# but I do not like that idea, so for now I disable ThreadSanitizer.
	#
	# https://stackoverflow.com/questions/77850769/fatal-threadsanitizer-unexpected-memory-mapping-when-running-on-linux-kernels
	#
	ENABLE_THREAD_SANITIZER Off
	ENABLE_ADDRESS_SANITIZER Off
	ENABLE_LEAK_SANITIZER Off
)
