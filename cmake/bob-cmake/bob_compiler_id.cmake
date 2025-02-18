#
# Compiler identification
#

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(BOB_COMPILER_CLANG On)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(BOB_COMPILER_GCC On)
elseif(MSVC)
	set(BOB_COMPILER_MSVC On)
else()
	bob_error("unsupported compiler.")
endif()
