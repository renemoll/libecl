
option(BOB_COVERAGE "Enable code coverage target creation" Off)

if (BOB_COVERAGE)
	find_program(GCOVR_PATH gcovr)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		#
		# Following the instructions from: https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
		#

		bob_info("generating llvm coverage report.")
		add_compile_options(
			# -fprofile-arcs
			# -ftest-coverage
			# -fno-elide-constructors
			-fprofile-instr-generate	# Generate instrumented code to collect execut6ion counts into default.profraw file
			-fcoverage-mapping			# Generate coverage mapping to enable code coverage analysis
			-fcoverage-mcdc				# Modified Condition/Decision Coverage (MC/DC)
		)
		add_link_options(
			-fprofile-instr-generate	# Generate instrumented code to collect execut6ion counts into default.profraw file
		)
	else()
		#
		# Based on: https://gcovr.com/en/stable/guide/compiling.html
		#

		bob_info("generating gcov coverage report.")
		add_compile_options(
			-O0												# Disable optimizations when generating test-coverage
			--coverage										# Synonym for -fprofile-arcs -ftest-coverage & -lgcov
			# -fprofile-arcs								# Instrument code to produce gcov data files (*.gcda)
			# -ftest-coverage								# Produce gcov notes files (*.gcno)
			$<$<CXX_COMPILER_ID:GNU>:-fprofile-abs-path>	# Use absolute instead of relative paths in the coverage data
		)
		add_link_options(
			--coverage
		)
	endif()
endif()

function(bob_create_coverage_report_target)
	if (NOT BOB_COVERAGE)
		return()
	endif()

	set(options NONE)
	set(oneValueArgs NAME)
	set(multiValueArgs RUNNER)
	cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${oneValueArgs}" "${multiValueArgs}")

	set(output_folder "${PROJECT_BINARY_DIR}/coverage_${arg_NAME}")
	bob_info("generating coverage report in: ${output_folder}")

	add_custom_target(
		${arg_NAME}
		COMMAND
			${arg_RUNNER}
		COMMAND
			${CMAKE_COMMAND} -E make_directory ${output_folder}
		COMMAND
			${GCOVR_PATH} -r ${PROJECT_SOURCE_DIR} --html-details --output "${output_folder}/index.html"
		COMMAND
			${GCOVR_PATH} -r ${PROJECT_SOURCE_DIR}
		WORKING_DIRECTORY
			${PROJECT_BINARY_DIR}
		DEPENDS
			${arg_RUNNER}
	)
endfunction()
