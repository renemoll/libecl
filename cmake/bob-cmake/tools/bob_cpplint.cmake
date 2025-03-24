#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# cpplint integration
#

option(BOB_CPPLINT "Execute `cpplint`" Off)

#
# bob_configure_cpplint
#
# Configure `cpplint` for the given `TARGET`.
#
function(bob_configure_cpplint TARGET)
	find_program(CPPLINT_EXE cpplint)

	if (BOB_CPPLINT)
		if (NOT CPPLINT_EXE)
			bob_error("request for cpplint failed as the executable could not be found")
		endif()

		bob_info("enabling cpplint for '${TARGET}'")

		set(CPPLINT_OPTIONS "")
		set_target_properties(${TARGET}
			PROPERTIES
				CXX_CPPLINT "${CPPLINT_EXE};${CPPLINT_OPTIONS}"
		)
	endif()
endfunction()
