#
# cppcheck integration
#

option(BOB_CPPCHECK "Execute `cppcheck`" On)

#
# Configure `cppcheck` for the given `TARGET`.
#
# Todo:
#  - set the correct standard version.
#

function(bob_configure_cppcheck TARGET)
	find_program(BOB_CPPCHECK_EXE NAMES cppcheck)

	if (BOB_CPPCHECK AND NOT BOB_CPPCHECK_EXE)
		message(WARNING "[Bob] request for `cppcheck` failed as the executable could not be found.")
	endif()

	if (BOB_CPPCHECK_EXE AND BOB_CPPCHECK)
		message(STATUS "[Bob] enabling `cppcheck` for `${TARGET}`")

		set(options
			"--enable=all"
			"-i${PROJECT_SOURCE_DIR}/build"
			"--inline-suppr"
			"--suppress=unmatchedSuppression"
			"--suppressions-list=${PROJECT_SOURCE_DIR}/.cppcheck_suppressions"
		)

		set_target_properties(${TARGET}
			PROPERTIES
				C_CPPCHECK "cppcheck;--std=c11;{$options}"
				CXX_CPPCHECK "cppcheck;--std=c++20;{$options}"
		)
	else()
		set_target_properties(${TARGET}
			PROPERTIES
				C_CPPCHECK ""
				CXX_CPPCHECK ""
		)
	endif()
endfunction()
