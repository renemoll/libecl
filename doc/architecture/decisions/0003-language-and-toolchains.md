# 3. Language and toolchains

Date: 2026-10-07

## Status

Accepted

## Context

Which C++ version(s) and toolchains should be supported by this library?

## Decision

**C++ language support**

This library will use C++20 as the baseline standard. New C++23 features may be used, with backward compatibility preserved.

Language features which are still in a draft stage will not be used.

**Toolchains**

This decision is based on what I use and see being used in the embedded C++ development community.

* Supported toolchains include: GCC, Clang (both can target PC and embedded platforms), Arm GNU Toolchain.
* MSVC is out of scope at this time due to limited use.

For each toolchain, the currently supported versions are selected and included in the CI builds. This is updated at least once per year.

**Embedded platform support**

This library will be tested with ARM Cortex-M series microcontrollers, including popular variants such as Cortex-M4, Cortex-M7, and Cortex-M33.

## Consequences

* Building with multiple toolchains and versions shows portability and helps catch toolchain-specific issues early.
* Different toolchains and versions will have different behaviours, level of standards compliance, and performance characteristics, which will show during development and testing.
* Building and testing with multiple toolchains and versions requires a robust CI setup.

**Toolchain support**

At the time of writing, the following toolchain versions are supported:

* For GCC (x64 and bare metal): 13, 14, 15, 16
* For Clang (x64 and bare metal): 20, 21, 22, 23
* For Arm GNU Toolchain: 13, 14, 15

The aim is to always use the latest version of a specific release series in the CI builds.
