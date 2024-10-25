
option(BOB_COVERAGE "" Off)

if (BOB_COVERAGE)
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
