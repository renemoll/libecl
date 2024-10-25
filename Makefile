
.PHONY: clang-coverage
clang-coverage:
	CC=clang CXX=clang++ cmake --workflow --preset coverage

.PHONY: clang-unittest
clang-unittest:
	CC=clang CXX=clang++ cmake --workflow --preset unittest

.PHONY: gcc-coverage
gcc-coverage:
	cmake --workflow --preset coverage

.PHONY: gcc-unittest
gcc-unittest:
	cmake --workflow --preset unittest
