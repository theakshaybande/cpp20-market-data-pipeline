# C++20 Market Data Pipeline

Minimal C++20 project scaffold for a market data parsing and processing pipeline.

## What It Does

- Pipeline: parse, normalize, and aggregate market data events
- Parsing: efficient tokenization with `string_view` and `from_chars`
- Aggregation: basic stats and stream handling

## Why C++20

- `concepts` for clear compile-time constraints
- `string_view` and `from_chars` for low-overhead parsing
- `format` for structured output
- `atomics` for lock-free queues and counters

## Build

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Layout

- `include/` public headers
- `src/` demo executable
- `tests/` unit tests

## Next Improvements

- SIMD parsing
- batch ingestion
- allocator tuning
- real feed handler
