#
# Compiler configuration
#

# Generate colourized diagnostic warnings.
if (BOB_COMPILER_CLANG)
	add_compile_options(
		-fcolor-diagnostics
	)
elseif(BOB_COMPILER_GCC)
	add_compile_options(
		-fdiagnostics-color=always
	)
else()
	bob_error("unsupported compiler.")
endif()
