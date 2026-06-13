#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

add_executable(benchmark_queue_spsc
    main.cpp
)

target_compile_options(benchmark_queue_spsc
    PRIVATE
        -O3
        -march=native
)

target_link_libraries(benchmark_queue_spsc
    PRIVATE
        benchmark::benchmark_main
        pthread
)

bob_configure_target(benchmark_queue_spsc
	ENABLE_CLANG_TIDY Off
	ENABLE_THREAD_SANITIZER Off
	ENABLE_ADDRESS_SANITIZER Off
	ENABLE_LEAK_SANITIZER Off
    ENABLE_UNDEFINED_BEHAVIOUR_SANITIZER Off
)
