
option(BOB_COVERAGE "Enable code coverage target creation" Off)

if (BOB_COVERAGE)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		#
		# Following the instructions from: https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
		#

		bob_info("generating llvm coverage report.")

		find_program(LLVM_PROFDATA_APP llvm-profdata)
		find_program(LLVM_COV_APP llvm-cov)

		add_compile_options(
			# -fprofile-arcs
			# -ftest-coverage
			# -fno-elide-constructors
			-fprofile-instr-generate	# Generate instrumented code to collect execution counts into default.profraw file
			-fcoverage-mapping			# Generate coverage mapping to enable code coverage analysis
			-fcoverage-mcdc				# Modified Condition/Decision Coverage (MC/DC)
		)
		add_link_options(
			-fprofile-instr-generate	# Generate instrumented code to collect execution counts into default.profraw file
		)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		#
		# Based on: https://gcovr.com/en/stable/guide/compiling.html
		#

		bob_info("generating gcov coverage report.")

		find_program(GCOVR_APP gcovr)

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
	else()
		bob_error("unsupported compiler")
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

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_custom_target(
			${arg_NAME}
			COMMAND
				${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${arg_NAME}.profraw" $<TARGET_FILE:${arg_RUNNER}>
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${output_folder}
			COMMAND
				${LLVM_PROFDATA_APP} merge -sparse "${arg_NAME}.profraw" -o "${arg_NAME}.profdata"
			COMMAND
				${LLVM_COV_APP} show
					-ignore-filename-regex=".*[/\]tests[/\].*"
					-show-mcdc
					-show-line-counts-or-regions
					-format=html
					-instr-profile="${arg_NAME}.profdata"
					$<TARGET_FILE:${arg_RUNNER}>
					> "${output_folder}/index.html"
			COMMAND
				${LLVM_COV_APP} report
					-ignore-filename-regex=".*[/\]tests[/\].*"
					-show-mcdc-summary
					-instr-profile="${arg_NAME}.profdata"
					$<TARGET_FILE:${arg_RUNNER}>
					> "${output_folder}/report.txt"
			WORKING_DIRECTORY
				${PROJECT_BINARY_DIR}
			DEPENDS
				${arg_RUNNER}
		)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_custom_target(
			${arg_NAME}
			COMMAND
				${arg_RUNNER}
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${output_folder}
			COMMAND
				${GCOVR_APP} -r ${PROJECT_SOURCE_DIR} --html-details --output "${output_folder}/index.html"
			COMMAND
				${GCOVR_APP} -r ${PROJECT_SOURCE_DIR}
			WORKING_DIRECTORY
				${PROJECT_BINARY_DIR}
			DEPENDS
				${arg_RUNNER}
		)
	else()
		bob_error("unsupported compiler")
	endif()
endfunction()
