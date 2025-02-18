find_package(Catch2 3 REQUIRED)
include(Catch)

add_executable(unittests
	test_bus_scheduler.cpp
	test_fifo.cpp
)

target_link_libraries(unittests
	PRIVATE
		ecl
		Catch2::Catch2WithMain
)

catch_discover_tests(unittests)
bob_create_coverage_report(
	NAME
		ecl-coverage
	RUNNER
		unittests
)
