#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# cppcheck integration
#

option(BOB_CPPCHECK "Execute `cppcheck`" On)

#
# bob_configure_cppcheck
#
# Configure `cppcheck` for the given `TARGET`.
#
function(bob_configure_cppcheck TARGET)
	find_program(CPPCHECK_EXE NAMES cppcheck)

	if (BOB_CPPCHECK)
		if (NOT CPPCHECK_EXE)
			bob_error("request for cppcheck failed as the executable could not be found")
		endif()

		bob_info("enabling cppcheck for '${TARGET}'")

		set(CPPCHECK_OPTIONS
			"--enable=all"
			"--force"
			"--library=gnu"
			"--language=c++"
			"-i${PROJECT_SOURCE_DIR}/build"
			"--inline-suppr"
			"--suppressions-list=${PROJECT_SOURCE_DIR}/.cppcheck_suppressions"
		)

		set_target_properties(${TARGET}
			PROPERTIES
				C_CPPCHECK "${CPPCHECK_EXE};--std=c11;${CPPCHECK_OPTIONS}"
				CXX_CPPCHECK "${CPPCHECK_EXE};--std=c++20;${CPPCHECK_OPTIONS}"
		)
	endif()
endfunction()
