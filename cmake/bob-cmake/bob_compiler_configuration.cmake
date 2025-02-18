#
# Compiler configuration
#

# Generate colourized diagnostic warnings.
add_compile_options(
    $<BOB_COMPILER_CLANG:-fcolor-diagnostics>
    $<BOB_COMPILER_GCC:-fdiagnostics-color=always>
)
