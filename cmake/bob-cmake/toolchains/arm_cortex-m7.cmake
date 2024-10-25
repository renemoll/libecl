#
# Cortex-M7 specific compiler configuration
#

add_compile_options(
	${ARM_API}
	-ffunction-sections				# Place each function into it's own section.
	-fdata-sections					# Place each data element into it's own section.
)

add_link_options(
	${ARM_API}
	-nostartfiles
	-nostdlib
	--specs=nosys.specs				# Use stubs for C syscalls.
	LINKER:--gc-sections			# Garbage collection using the unique function and data sections.
	LINKER:--build-id=uuid			# Generate a unique identiefier for each build and store it in a specific section (.note.gnu.build-id).
	LINKER:--cref					# Generate a cross reference table in the MAP file, listing symbols and their source file(s).
	LINKER:--no-warn-rwx-segment
)

include("${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi-gcc.cmake")
