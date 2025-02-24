
add_library(ecl
	INTERFACE
)
target_include_directories(ecl
	INTERFACE
		"${PROJECT_SOURCE_DIR}/include"
)

if(PROJECT_IS_TOP_LEVEL)
	bob_configure_clang_tidy(ecl)
	bob_configure_compiler_warnings(ecl)
	bob_configure_sanitizers(ecl)
endif()
