
set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
set(CTEST_BINARY_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build/ninja-coverage")

ctest_start(NightlyCoverage)
ctest_update()

include(ProcessorCount)
ProcessorCount(nproc)

ctest_test(
	PARALLEL_LEVEL "${nproc}"
	RETURN_VALUE test_result
	OUTPUT_JUNIT "${CTEST_BINARY_DIRECTORY}/junit.xml"
)

find_program(GCOV NAMES gcov)
set(CTEST_COVERAGE_COMMAND "${GCOV}")
set(CTEST_COVERAGE_EXTRA_FLAGS
	--hash-filenames
	--long-file-names
)
string(REPLACE ";" " " CTEST_COVERAGE_EXTRA_FLAGS "${CTEST_COVERAGE_EXTRA_FLAGS}")
ctest_coverage()
