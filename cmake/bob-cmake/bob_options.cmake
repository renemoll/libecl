#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

# option(BOB_USE_DEBUG_SYMBOLS	"Embed debug symbols" ON)
# option(BOB_USE_EXCEPTIONS		"Allow exceptions" OFF)
# option(BOB_USE_RTTI				"Generate run-time type identification (RTTI)" OFF)

# function(bob_configure_options target)
# 	if(BOB_COMPILER_GCC OR BOB_COMPILER_CLANG)
# 		target_compile_options(${target}
# 			INTERFACE
# 				$<$<BOOL:${BOB_USE_DEBUG_SYMBOLS}>:-gdwarf-5>
# 				$<$<BOOL:${BOB_USE_EXCEPTIONS}>:-fexceptions>
# 				$<$<NOT:$<BOOL:${BOB_USE_EXCEPTIONS}>>:-fno-exceptions>
# 				$<$<NOT:$<BOOL:${BOB_USE_EXCEPTIONS}>>:-fno-unwind-tables>
# 				$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<BOOL:${BOB_USE_RTTI}>>:-frtti>
# 				$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<BOOL:${BOB_USE_RTTI}>>>:-fno-rtti>
# 		)
# 	else()
# 		bob_error("unsupported compiler")
# 	endif()
# endfunction()
