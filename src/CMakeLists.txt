
add_library(ecl
	STATIC
		"${CMAKE_CURRENT_LIST_DIR}/communication/bus_scheduler.cpp"
)
target_include_directories(ecl
	PUBLIC
		"${PROJECT_SOURCE_DIR}/include"
)

if(PROJECT_IS_TOP_LEVEL)
	bob_configure_clang_tidy(ecl)
	bob_configure_compiler_warnings(ecl)
	bob_configure_sanitizers(ecl)
endif()
