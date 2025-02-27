#
# flawfinder integration
#

option(BOB_FLAWFINDER "Execute `flawfinder`" Off)

if (BOB_FLAWFINDER)
	find_program(FLAWFINDER_EXE flawfinder)

	if (FLAWFINDER_EXE)
		bob_info("enabling flawfinder")

        set(OPTIONS
            "${PROJECT_SOURCE_DIR}/include"
            "${PROJECT_SOURCE_DIR}/src"
            "${PROJECT_SOURCE_DIR}/test"
        )

        add_custom_target(flawfinder
            ALL
            COMMAND
                ${CMAKE_COMMAND} -E ${FLAWFINDER_EXE} ${OPTIONS}
            WORKING_DIRECTORY
                 ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT
                "flawfinder"
			VERBATIM

        )
		# set(CPPLINT_OPTIONS
		# )
		# set_property(
		# 	TARGET
		# 		${TARGET}
		# 	PROPERTY
		# 		CXX_CPPLINT
		# 		"${CPPLINT_EXE};${CPPLINT_OPTIONS}"
		# )

    else()
		bob_error("request for flawfinder failed as the executable could not be found")
	endif()
endif()
