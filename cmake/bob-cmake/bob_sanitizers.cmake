#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# Compiler sanitizer configuration
#
# Todo:
# - Hardware-assisted AddressSanitizer 
#
# Note:
# - Disabled MemorySanitizer as this requires the complete code base
#   (inc external dependencies such as libc++) to be compiled with it.
#

option(BOB_SANITIZE_ADDRESS "Enable AddressSanitizer" On)
option(BOB_SANITIZE_LEAK "Enable LeakSanitizer" On)
option(BOB_SANITIZE_UNDEFINED "Enable UndefinedBehaviorSanitizer" On)
# option(BOB_SANITIZE_MEMORY "Enable MemorySanitizer" Off)
option(BOB_SANITIZE_THREAD "Enable ThreadSanitizer" Off)

#
# bob_configure_sanitizers
#
# Enable compiler sanitizers for the given `TARGET`. The specific sanitizers to enable are
# determined by the `BOB_SANITIZE_*` options.
#
function(bob_configure_sanitizers TARGET)

	#
	# Determine which sanitizers to enable
	#
	set(SANITIZERS "")

	if (BOB_SANITIZE_ADDRESS)
		list(APPEND SANITIZERS "address")
	endif()

	if (BOB_SANITIZE_LEAK)
		list(APPEND SANITIZERS "leak")
	endif()

	if (BOB_SANITIZE_UNDEFINED)
		list(APPEND SANITIZERS "undefined")
	endif()

	if (BOB_SANITIZE_THREAD)
		if("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
			bob_error("ThreadSanitizer cannot be combined with AddressSanitizer or LeakSanitizer")
		else()
			list(APPEND SANITIZERS "thread")
		endif()
	endif()

	# if (BOB_SANITIZE_MEMORY)
	# 	if (BOB_COMPILER_CLANG)
	# 		list(APPEND SANITIZERS "memory")
	# 	else()
	# 		bob_info("MemorySanitizer is only supported by Clang")
	# 	endif()
	# endif()

	if (SANITIZERS)
		#
		# Update compiler options
		#

		if (NOT BOB_COVERAGE)
			add_compile_options( 		# TODO: only apply these options to the given `TARGET`
				-O1						# Recommended for "reasonable performance"
				-fno-omit-frame-pointer # For better stack traces
				-g						# For file names and line numbers
			)
		endif()

		#
		# Add the sanitizer(s) to the given `TARGET`
		#

		list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

		target_compile_options(${TARGET}
			INTERFACE
				-fsanitize=${LIST_OF_SANITIZERS}
		)
		target_link_options(${TARGET}
			INTERFACE
				-fsanitize=${LIST_OF_SANITIZERS}
		)
	endif()
endfunction()

# #
# # bob_sanitize_enable_sanitizer
# #
# # Enable compiler sanitizers for the given `TARGET`. The specific sanitizers to enable are
# # determined by the function arguments. This function is intended when you want to deviate
# # from the global sanitizer configuration (e.g. enable ThreadSanitizer for a specific target).
# #
# function(bob_sanitize_enable_sanitizer TARGET SANITIZERS)
# 	# cmake_parse_arguments(args "" "" "SANITIZERS" ${ARGN})
# 	message(STATUS "Enabling sanitizers '${SANITIZERS}' for target '${TARGET}'")
#
# 	target_compile_options(${TARGET}
# 		PUBLIC
# 			-fsanitize=${SANITIZERS}
# 	)
# 	target_link_options(${TARGET}
# 		PUBLIC
# 			-fsanitize=${SANITIZERS}
# 	)
# endfunction()
