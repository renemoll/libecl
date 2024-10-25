#
# Cross-compiler configuration for ARM Cortex-M7
#

set(CMAKE_SYSTEM_NAME 		Generic)
set(CMAKE_SYSTEM_PROCESSOR 	arm)

#
# Find my compiler
#

set(CMAKE_C_COMPILER   "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(TOOLCHAIN_SIZE     "arm-none-eabi-size"    CACHE STRING "arm-none-eabi-size")
set(TOOLCHAIN_OBJDUMP  "arm-none-eabi-objdump" CACHE STRING "arm-none-eabi-objdump")

#
# CMAKE configuration
#
# These variables change the behaviour of where various `find_*` commands search:
# - CMAKE_FIND_ROOT_PATH specified where the target environment can be found. 
# - CMAKE_FIND_ROOT_PATH_MODE_PROGRAM influences `find_program`.
# - CMAKE_FIND_ROOT_PATH_MODE_LIBRARY influences `find_library`.
# - CMAKE_FIND_ROOT_PATH_MODE_INCLUDE influences `find_path` and `find_file`.
# - CMAKE_FIND_ROOT_PATH_MODE_PACKAGE influences `find_package`.
#
# Valid values:
# - NEVER	`CMAKE_FIND_ROOT_PATH` will be ignored, thus only searching the host system.
# - ONLY	Only paths within target environment (`CMAKE_FIND_ROOT_PATH`) are searched.
# - BOTH	Both target (first) and host (second) environments are searched.
#

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#
# Tell CMAKE to build libraries when detecting functionality as target executables
# cannot be executed on the host environment.
#

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
