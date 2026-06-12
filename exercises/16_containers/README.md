# 16_containers — Vectors that move house, maps that insert behind your back

The containers are the best-worn part of the standard library — and the
part where "valid yesterday" pointers, iterators, and references quietly
die. This module is mostly about **invalidation rules**: knowing, per
container, which operations move your data out from under you.

## What this module covers

- **`std::vector` growth.** `size` vs `capacity`; growth reallocates —
  every element moves (module 09's `noexcept` story), and **every
  pointer, reference, and iterator into the vector dies**. `reserve()`
  buys stability up front.
- **Erasing while iterating.** `erase(it)` shifts everything left and
  invalidates from `it` onward; the loop idiom is `it = v.erase(it)`
  *or else* `++it`. (C++20's `std::erase_if(v, pred)` makes the whole
  dance one line — module 17.)
- **`std::map` vs `std::unordered_map`.** Red-black tree (sorted keys,
  O(log n), node-stable) vs hash table (O(1) average, rehashes).
  **`operator[]` inserts a default-constructed value for missing keys** —
  a read that mutates. Query with `.find()` / `.contains()` *(C++20)*.
- **Insertion API zoo.** `insert` (no-op if key exists, tells you),
  `operator[]` (overwrites value), `emplace`, and `try_emplace`
  *(C++17)* — constructs in place only if the key is new.
- **Node stability.** `map`/`set` never move elements; pointers and
  references to elements survive any insert/erase (except your own).
  `unordered_map` rehash invalidates *iterators* but **not** pointers/
  references to elements. `vector` invalidates everything on growth.
- **`std::span`** *(C++20)* — `string_view`'s sibling for any contiguous
  data: one non-owning `{pointer, size}` parameter type that accepts
  `vector`, `std::array`, and C arrays alike.

## Version notes

| Feature | Standard |
|---|---|
| `unordered_map` / `unordered_set` | C++11 |
| `try_emplace`, `insert_or_assign` | C++17 |
| `contains()` | C++20 |
| `std::span` | C++20 |
| `std::erase` / `std::erase_if` free functions | C++20 |
