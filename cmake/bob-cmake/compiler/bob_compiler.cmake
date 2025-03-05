#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# Compiler identification
#

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(BOB_COMPILER_CLANG On)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(BOB_COMPILER_GCC On)
elseif(MSVC)
	set(BOB_COMPILER_MSVC On)
else()
	bob_error("unsupported compiler.")
endif()


#
# Compiler configuration
#

# Generate colourized diagnostic warnings.
if (BOB_COMPILER_CLANG)
	add_compile_options(
		-fcolor-diagnostics
	)
elseif(BOB_COMPILER_GCC)
	add_compile_options(
		-fdiagnostics-color=always
	)
else()
	bob_error("unsupported compiler.")
endif()

#
# Compiler warnings
#

include(bob_compiler_warnings)
