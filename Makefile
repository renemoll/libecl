#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: CC0-1.0
#

.PHONY: clang-coverage
clang-coverage:
	CC=clang CXX=clang++ cmake --workflow --preset coverage

.PHONY: clang-unittest
clang-unittest:
	CC=clang CXX=clang++ cmake --workflow --preset unittest-debug

.PHONY: coverage
gcc-coverage:
	cmake --workflow --preset coverage

.PHONY: unittest
gcc-unittest:
	cmake --workflow --preset unittest-debug
