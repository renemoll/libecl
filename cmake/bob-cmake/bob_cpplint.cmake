#
# cpplint integration
#

option(BOB_CPPLINT "Execute `cpplint`" Off)

function(bob_configure_cpplint TARGET)
	find_program(CPPLINT_EXE cpplint)

	if (BOB_CPPLINT AND NOT CPPLINT_EXE)
		bob_error("request for cpplint failed as the executable could not be found")
	endif()

	if (BOB_CPPLINT AND CPPLINT_EXE)
		bob_info("enabling cpplint for '${TARGET}'")

		set(CPPLINT_OPTIONS
		)
		set_property(
			TARGET
				${TARGET}
			PROPERTY
				CXX_CPPLINT
				"${CPPLINT_EXE};${CPPLINT_OPTIONS}"
		)
	endif()
endfunction()
