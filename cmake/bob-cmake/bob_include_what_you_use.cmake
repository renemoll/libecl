#
# include-what-you-use integration
#

option(BOB_IWYU "Execute `include-what-you-use`" On)

function(bob_configure_iwyu TARGET)
	find_program(IWYU_EXE include-what-you-use)

	if (BOB_IWYU AND NOT IWYU_EXE)
		bob_error("request for include-what-you-use failed as the executable could not be found")
	endif()

	if (BOB_IWYU AND IWYU_EXE)
		bob_info("enabling include-what-you-use for '${TARGET}'")

		set(IWYU_OPTIONS
			"-Xiwyu" "--quoted_includes_first"
			"-Xiwyu" "--transitive_includes_only"
			"-Xiwyu" "--cxx17ns"
			# Avoid extra Clang diagnostics from include-what-you-use (got some unhelpful false positives)
			"-w"
		)
		set_property(
			TARGET
				${TARGET}
			PROPERTY
				CXX_INCLUDE_WHAT_YOU_USE
				"${IWYU_EXE};${IWYU_OPTIONS}"
		)
	endif()
endfunction()
