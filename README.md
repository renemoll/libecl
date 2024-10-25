# Embedded components library (ECL)

Just a place to keep components/code I re-use.

# Notes

Using:
* [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html)
* [Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/)

# Local development environment

You can use the provided devcontainer or setup your own.

```bash
pip install -r requirements.txt
pre-commit install --hook-type commit-msg
```

# Compiling & testing

This project provides CMake workflows to easy building from source code.

List available workflows:
```bash
cmake --workflow --list-presets
```

For example, tets coverage:
```bash
cmake --workflow --preset coverage
```



## OLD

Compiling for host:
```bash
cmake --preset host-debug
cmake --build --preset host-debug
```

Generating code coverage
```bash
cmake --preset host-debug-coverage
cmake --build --preset host-debug-coverage
ctest --preset coverage
```




# Scratchpad





```bash
ctest --preset host
```

```bash
cmake --preset host
clear; cmake --build --preset host-debug && ctest --preset host

gcovr --gcov-executable "llvm-cov gcov" -e .  --verbose -r ../..  .


cmake -S . -B build
cmake --build ./build/
./build/common/tests/unittests

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build/
gcovr -r ..  . -> Now ok with  gcov-14..
gcovr -r .. -e ../common/tests/ --branches   .

gcovr -r .. .
gcovr -r .. --html-details coverage.html .

CC=clang CXX=clang++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build/
cd build
./common/tests/unittests
llvm-profdata merge -sparse default.profraw -o default.profdata
llvm-cov show -ignore-filename-regex=".*[/\]tests[/\].*" -show-mcdc -format=text ./common/tests/unittests -instr-profile=default.profdata 
llvm-cov show -ignore-filename-regex=".*[/\]tests[/\].*" -show-mcdc -format=html ./common/tests/unittests -instr-profile=default.profdata 
llvm-cov report -ignore-filename-regex=".*[/\]tests[/\].*" -show-mcdc-summary ./common/tests/unittests -instr-profile=default.profdata 
```

