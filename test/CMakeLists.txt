
add_executable(unittests
	test_fifo.cpp
)
target_link_libraries(unittests
	PRIVATE
		ecl
		Catch2::Catch2WithMain
)
catch_discover_tests(unittests)
bob_create_coverage_report_target(
	NAME
		ecl-coverage
	RUNNER
		unittests
)
