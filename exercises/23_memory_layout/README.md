# 23_memory_layout — Where the bytes actually are

C++'s superpower is that object layout is *knowable*: `sizeof`,
`alignof`, padding, and allocation behavior are all things you can
reason about, measure, and exploit. This module makes layout visible.

## What this module covers

- **Alignment & padding.** Every type has an alignment requirement
  (`alignof`); structs insert padding so each member lands on its
  alignment, and the struct's own size rounds up to its strictest
  member's alignment. **Member order changes object size** — sort
  members large-to-small to pack tight. Arrays multiply the waste.
- **`alignas`** *(C++11)* — demanding more alignment than a type needs:
  SIMD loads, cache-line isolation (`alignas(64)` — see module 25's
  false-sharing note), page boundaries.
- **Placement new** — constructing an object *into memory you already
  have*: `new (ptr) T(args)`. The other half of the contract: you must
  call `t->~T()` yourself. This is the machinery beneath `vector`
  (raw capacity + constructed prefix), arenas, and `std::optional`.
- **Empty members & `[[no_unique_address]]`** *(C++20)*. An empty class
  still has `sizeof == 1` (distinct objects need distinct addresses) —
  as a member, that 1 byte plus padding can cost a whole word.
  `[[no_unique_address]]` lets an empty member truly occupy zero bytes —
  the modern spelling of the empty-base optimization that made module
  10's stateless-deleter `unique_ptr` pointer-sized.
- **Counting allocations.** Replacing global `operator new` reveals what
  the library really does: vector growth chains, SSO, and
  `make_shared`'s fused single allocation.
- **Register-map overlays (MMIO).** Firmware matches a struct to a
  datasheet's byte-offset table and audits it with `offsetof` +
  `static_assert` — which only works for standard-layout types. The
  compiler inserts just the padding alignment demands, so datasheet
  holes become explicit reserved members (not `#pragma pack`, which
  hurts codegen on strict-alignment cores).
- **Endianness.** Multi-byte integers have a byte *order* in memory;
  wire protocols are usually big-endian while your host probably isn't.
  `std::endian` *(C++20)* names the host order; shift-and-mask
  arithmetic (de)serializes portably because arithmetic is defined on
  values, not bytes.

## Version notes

| Feature | Standard |
|---|---|
| `alignas` / `alignof` | C++11 |
| `std::byte` | C++17 |
| `[[no_unique_address]]` | C++20 |
| `std::construct_at` / `destroy_at` (constexpr-friendly placement) | C++20 |
| `std::launder` (the dark corners we skip) | C++17 |
| `offsetof` (inherited from C, `<cstddef>`) | C++98 |
| `std::endian` (`<bit>`) | C++20 |
| `std::byteswap` (mentioned) | C++23 |
