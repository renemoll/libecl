#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# include-what-you-use integration
#

option(BOB_IWYU "Execute `include-what-you-use`" On)

#
# bob_configure_iwyu
#
# Configure `include-what-you-use` for the given `TARGET`.
#
function(bob_configure_iwyu TARGET)
	find_program(IWYU_EXE include-what-you-use)

	if (BOB_IWYU)
		if (NOT IWYU_EXE)
			bob_error("request for include-what-you-use failed as the executable could not be found")
		endif()

		bob_info("enabling include-what-you-use for '${TARGET}'")

		set(IWYU_OPTIONS
			"-Xiwyu" "--quoted_includes_first"
			"-Xiwyu" "--transitive_includes_only"
			"-Xiwyu" "--cxx17ns"
			# Avoid extra Clang diagnostics from include-what-you-use (got some unhelpful false positives)
			"-w"
		)
		set_target_properties(${TARGET}
			PROPERTIES
				CXX_INCLUDE_WHAT_YOU_USE "${IWYU_EXE};${IWYU_OPTIONS}"
		)
	endif()
endfunction()
