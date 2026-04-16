#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: CC0-1.0
#

.PHONY: clang-coverage
clang-coverage:
	cmake --workflow --preset coverage-clang-debug

.PHONY: clang-unittest
clang-unittest:
	cmake --workflow --preset unittest-clang-debug

.PHONY: coverage
gcc-coverage:
	cmake --workflow --preset coverage-gcc-debug

.PHONY: unittest
gcc-unittest:
	cmake --workflow --preset unittest-gcc-debug

.PHONY: gcc-m7
gcc-m7:
	cmake --workflow --preset build-gcc-cortex-m7-debug

# .PHONY: benchmark
# benchmark:
# 	cmake --workflow --preset benchmark
