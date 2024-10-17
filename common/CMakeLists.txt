
add_library(ccl_common
	INTERFACE
)
target_include_directories(ccl_common
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory(tests)
endif()
