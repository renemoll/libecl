#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# Compiler configuration
#

if (BOB_COMPILER_CLANG)
	option(BOB_CLANG_WARN_EVERYTHING "Enable `-Weverything` for Clang" Off)
endif()

#
# bob_configure_compiler_warnings
#
# Apply compiler checks (warnings) for the given `TARGET`.
#
function(bob_configure_compiler_warnings TARGET)
	set(WARNINGS "")
	set(C_WARNINGS "")
	set(CXX_WARNINGS "")

	if (BOB_COMPILER_CLANG OR BOB_COMPILER_GCC)
		list(APPEND WARNINGS
			# General
			-Wall								# Enable warnings for common coding mistakes or potential errors.
			-Wextra								# Extensions for -Wall.
			-Werror								# Treat warnings as errors to fail the build in case of warnings.
			-Wpedantic							# Warn about non-standard C/C++.
			# (Type) conversion
			-Wconversion						# Warn about implicit type conversions which (may) change the value.
			-Wsign-conversion					# Warn about implicit sign conversions.
			-Wdouble-promotion					# Warn about floats being implicitly converted to doubles.
			-Wfloat-equal						# Warn about floating point values used in equality tests.
			-Wcast-qual							# Warn when casting removes a type qualifier from a pointer.
			-Wcast-align						# Warn when casting a pointers changes the alignment of the pointee.
			-Wstrict-overflow=2					# Warn about optimizations where signed overflow is assumed not to occur.
			# Misc
			-Wshadow							# Warn about duplicated variable names.
			-Wswitch-enum						# Warn about switch statements not using all possible enum values.
			-Wimplicit-fallthrough				# Warn about implicit, un-annotated, fallthrough.
			-Wnull-dereference					# Warn about possible null pointer dereference code paths.
			-Wundef								# Warn when undefined macros are used (implicit conversion to 0.)
			-Wunused							# Warn about any unused parameter/function/variable/etc...
			-Wmisleading-indentation			# Warn about indentation giving the impression of scope.
			-Winline							# Warn when desired inlining is not possible.
			-Wzero-as-null-pointer-constant		# Warn about the use of 0 as nullptr.
			# Strings related
			-Wvla								# Warn about variable-length arrays being used.
			-Wwrite-strings						# Warn when attempting to write to a string constant.
			-Wformat=2							# Verify printf/scanf/.. arguments and format strings match.
		)

		list(APPEND C_WARNINGS
			# (Type) conversion
			-Wbad-function-cast					# Warn about casts to function pointers.
			# Misc
			-Wstrict-prototypes					# Warn when a function declaration misses argument types.
		)

		list(APPEND CXX_WARNINGS
			# (Type) conversion
			-Wold-style-cast					# Warn about C-style casts.
			# Classes
			-Wnon-virtual-dtor					# Warn about base classes without virtual destructors.
			-Wctor-dtor-privacy					# Warn about classes which seemingly cannot be used.
			-Wsuggest-override					# Warn when a method overwriting a virtual method is not marked with override.
			-Woverloaded-virtual				# Warn when a derived function hides a virtual function of the base class.
		)
	endif()

	if (BOB_COMPILER_GCC)
		list(APPEND WARNINGS
			# (Type) conversion
			-Warith-conversion					# Warn about implicit type conversions during arithmetic operations.
			-Wcast-align=strict					# Warn when casting a pointers changes the alignment of the pointee.
			-Wshift-overflow=2					# Warn about left shifting a 1 into the sign bit.
			# Misc
			-Wduplicated-branches				# Warn about identical branches in if-else expressions.
			-Wduplicated-cond					# Warn about duplicated conditions in if-else expressions.
			-Wredundant-decls					# Warn about multiple declarations within the same scope.
			-Wlogical-op						# Warn about potential errors with logical operations.
			-Wtrampolines						# Warn about code to jump to a function, requiring an executable stack.
			-Warray-bounds=2					# Warns about invalid array indices.
			-Wstrict-null-sentinel				# Warn about the use of an uncasted NULL as sentinel.
			-Wtrivial-auto-var-init				# Warn about automatic variables which might be uninitialized.
			# Strings related
			-Wformat-truncation=2				# Warn when the output of sprintf/... might be truncated.
		)

		list(APPEND CXX_WARNINGS
			# (Type) conversion
			-Wuseless-cast						# Warn about casting to the same type.
		)
	elseif (BOB_COMPILER_CLANG)
		list(APPEND WARNINGS
			# (Type) conversion
			-Wshift-sign-overflow				# Warn about left shifting a 1 into the sign bit.
			-Wzero-as-null-pointer-constant		# Warn about using 0 as a null pointer.
			# Misc
			-Wshadow-all						# Additional shadowing checks.
			-Wconditional-uninitialized			# Warn about variables which might be uninitialized.
			-Wloop-analysis						# Warn about loop variables being manipulated/ignored/.. inside the loop
			# Strings related
			-Wformat-type-confusion				# Warn when an argument does match the format specified type.
		)

		if (BOB_CLANG_WARN_EVERYTHING)
			list(APPEND WARNINGS
				-Weverything					# Enable all diagnostic warnings.
			)
		endif()
	else()
		bob_error("unsupported compiler.")
	endif()

	target_compile_options(${TARGET}
		INTERFACE
			${WARNINGS}
			$<$<COMPILE_LANGUAGE:C>:${C_WARNINGS}>
			$<$<COMPILE_LANGUAGE:CXX>:${CXX_WARNINGS}>
	)


# 	set(BOB_COMPILER_WARNINGS_MSVC
# 		/permissive-										# Conform to the C++ standard.
# 		/W4													# Enable almost all warnings.
# 		/WX													# Treat warnings as errors.
# 		# MSVC syntax: w<level><warning code>
# 		/w14062												# 'identifier' in a switch of enum 'enumeration' is not handled.
# 		/w14242												# 'identifier': conversion from 'type1' to 'type1', possible loss of data.
# 		/w14254												# 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data.
# 		/w14263												# 'function': member function does not override any base class virtual member function
# 		/w14265												# 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
# 		/w14287												# 'operator': unsigned/negative constant mismatch
# 		/we4289												# non-standard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
# 		/w14296												# 'operator': expression is always 'boolean_value'
# 		/w14311												# 'variable': pointer truncation from 'type1' to 'type2'
# 		/w14388												# 'token' : signed/unsigned mismatch
# 		/w14545												# expression before comma evaluates to a function which is missing an argument list
# 		/w14546												# function call before comma missing argument list
# 		/w14547												# 'operator': operator before comma has no effect; expected operator with side-effect
# 		/w14549												# 'operator': operator before comma has no effect; did you intend 'operator'?
# 		/w14555												# expression has no effect; expected expression with side-effect
# 		/w14619												# pragma warning: there is no warning number 'number'
# 		/w14640												# Enable warning on thread un-safe static member initialization
# 		/w14668												# 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
# 		/w14800												# Implicit conversion from 'type' to bool. Possible information loss
# 		/w14826												# Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
# 		/w14928												# illegal copy-initialization; more than one user-defined conversion has been implicitly applied
# 		/w14946												# reinterpret_cast used between related classes: 'class1' and 'class2'
# 	)
endfunction()

# function(bob_configure_compiler_codegen TARGET)
# 	set(BOB_COMPILER_BEHAVIOUR_GNU_CLANG
# 		-fno-common													# Warn when global variables are not unique (and unintentional merged.)
# 		-fstack-usage												# Generate stack depth information.
# 		-fvisibility=hidden											# Sets the default symbol visibility to hidden.
# 		-fvisibility-inlines-hidden									# Sets the default symbol visibility to hidden for inline functions.
# 		-fwrapv														# Assume signed arithmetic may wrap around.
# 		$<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>			# Compile ASM as C++
# 		$<$<COMPILE_LANGUAGE:CXX>:-fdiagnostics-show-template-tree>	# Print template structures in a tree structure.
# 		-ftrivial-auto-var-init=zero								# Ensure automatic variables are always initialized.
# 	)

# 	set(BOB_COMPILER_BEHAVIOUR_GNU
# 		$<$<NOT:$<CONFIG:Release>>:-fvar-tracking-assignments>	# Attempt to improve debugging by annotating variable assignment.
# 	)

# 	set(BOB_COMPILER_BEHAVIOUR_MSVC
# 		/diagnostics:caret									# Indicate where a warning was found in a line.
# 		/Gy													# Enable function level linking, allowing unreferenced data/functions to be excluded.
# 		/sdl												# Enable additional security checks.
# 		/Zc:__cplusplus										# Enable __cplusplus to report the supported standard.
# 		/Zc:externConstexpr									# Enable extern constexpr variables.
# 		/Zc:lambda											# Enable conforming lambda processor.
# 		/Zc:inline											# Remove unreferenced data and functions.
# 		/Zc:referenceBinding								# Enforce reference binding rules.
# 		/Zc:rvalueCast										# Enforce type conversion rules.
# 		/Zc:throwingNew										# Assume operator new throws.
# 	)

# 	target_compile_options(${TARGET}
# 		INTERFACE
# 			$<$<CXX_COMPILER_ID:Clang>:${BOB_COMPILER_BEHAVIOUR_GNU_CLANG}>
# 			$<$<CXX_COMPILER_ID:GNU>:${BOB_COMPILER_BEHAVIOUR_GNU_CLANG}>
# 			$<$<CXX_COMPILER_ID:GNU>:${BOB_COMPILER_BEHAVIOUR_GNU}>
# 			$<$<CXX_COMPILER_ID:MSVC>:${BOB_COMPILER_BEHAVIOUR_MSVC}>
# 	)
# endfunction()
