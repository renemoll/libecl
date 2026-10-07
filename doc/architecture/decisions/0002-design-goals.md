# 2. Design goals

Date: 2026-10-06

## Status

Accepted

## Context

What are the driving factors for design decisions within this library?

This library is a collection of C++ components I frequently use for embedded application development. Via this library, I want to re-use, maintain, and improve these components over time.

## Decision

For this library, the following design goals are important:

1. Deterministic and predictable behaviour; Documentation and implementation should ensure that the behaviour of each component is well-defined and consistent, in both happy and unhappy flows.
2. Explicit error handling; Use explicit result types (ie. using adaptions of `optional` and `expected`) to handle error conditions without exceptions, keeping the control flow clear and predictable.
3. Explicit ownership and memory management; Both through language constructs (like move semantics) and through clear documentation of ownership responsibilities.
4. Clear and concise APIs; Using familiar C++ terminology and a single approach for common use-cases.
5. Low overhead; Avoid unnecessary runtime work (CPU time) and memory use (both code size and RAM usage).
6. Portability; Use standard C++ constructs to ensure the library can be used across different platforms and compilers.
7. No dynamic memory allocation within the library.

## Consequences

* Keep APIs small and focused; ensure that each component has well-defined responsibilities and interactions within the application flow.
* Clearly define, document, and check preconditions, postconditions, and invariants for each component, ensuring correct usage and predictable behaviour.
* Provide alternatives to standard types where necessary to meet the design goals, for example result types like `optional` and `expected` without exceptions.
* Use the type system effectively; Leverage C++'s strong typing to enforce correctness and prevent common errors at compile time.
* Utilize testing to verify that each component behaves correctly under various conditions and meets the design goals.
