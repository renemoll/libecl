#
# Clang-tidy integration
#

option(BOB_CLANG_TIDY "Execute `clang-tidy`" On)

#
# Configure `clang-tidy` for the given `TARGET`.
#

function(bob_configure_clang_tidy TARGET)
	find_program(BOB_CLANG_TIDY_EXE NAMES clang-tidy)

	if (NOT BOB_CLANG_TIDY_EXE AND BOB_CLANG_TIDY)
		message(WARNING "[Bob] request for clang-tidy failed as the executable could not be found.")
	endif()

	if (BOB_CLANG_TIDY_EXE AND BOB_CLANG_TIDY)
		message(STATUS "[Bob] enabling clang-tidy for '${TARGET}'.")

		set(CLANG_TIDY_CMD
			"${BOB_CLANG_TIDY_EXE}"
			"--config-file=${PROJECT_SOURCE_DIR}/.clang-tidy"
		)
		set_target_properties(${TARGET}
			PROPERTIES
				C_CLANG_TIDY "${CLANG_TIDY_CMD}"
				CXX_CLANG_TIDY "${CLANG_TIDY_CMD}"
		)
	else()
		set_target_properties(${TARGET}
			PROPERTIES
				C_CLANG_TIDY ""
				CXX_CLANG_TIDY ""
		)
	endif()
endfunction()
