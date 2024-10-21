
add_executable(unittests
	test_fifo.cpp
)
target_link_libraries(unittests
	PRIVATE
		ccl_common
		Catch2::Catch2WithMain
)
# catch_discover_tests(unittests)
