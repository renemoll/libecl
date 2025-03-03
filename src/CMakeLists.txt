#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

add_library(ecl
	STATIC
		"${CMAKE_CURRENT_LIST_DIR}/communication/bus_scheduler.cpp"
)
target_include_directories(ecl
	PUBLIC
		"${PROJECT_SOURCE_DIR}/include"
)

if(PROJECT_IS_TOP_LEVEL)
	bob_configure_compiler_warnings(ecl)
	bob_configure_sanitizers(ecl)
	
	bob_configure_clang_tidy(ecl)
	bob_configure_cppcheck(ecl)
	bob_configure_cpplint(ecl)
	bob_configure_iwyu(ecl)
endif()
