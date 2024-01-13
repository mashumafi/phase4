# phase4

A Strong Chess Engine using C++17 based on [Cosette](https://github.com/Tearth/Cosette).
This engine can also play a variant called "Slide 15" where squares slide along with their pieces.

## Goals

### Avoid singletons

This is a common approach in chess engine design to keep things simple and fast.
It leads to complex setups and difficult to embed engines in applications that are not a UCI client.

### Lightweight

Avoid dependencies, header-only library. The ability to just put `/include/phase4` into any project and use it right away.

### Allocations

Avoid allocations. Carefully choose `std` API and use `std::pmr` where avilable.

## Structure

The entire library is contained in `/include/phase4`.
