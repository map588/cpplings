# 10_raii_smart_ptrs — Ownership as a type, not a comment

RAII — *Resource Acquisition Is Initialization* — is the single most
important idiom in C++: tie a resource's lifetime to an object's lifetime,
and the destructor releases it on **every** exit path: returns, breaks,
exceptions mid-flight. Smart pointers are RAII applied to ownership of
heap objects, with the ownership *policy* encoded in the type.

## What this module covers

- **RAII.** Manual `acquire()`/`release()` pairs break the moment someone
  adds an early `return` between them. A guard object cannot forget.
- **`std::unique_ptr`** *(C++11)* — sole ownership, zero overhead (same
  size as a raw pointer, with the default deleter), move-only: copying is
  a compile error, moving transfers ownership and **nulls the source**
  (specified, unlike most moved-from states). Create with
  **`std::make_unique`** *(C++14)*. This replaces `std::auto_ptr`
  *(deprecated C++11, removed C++17)* — if you see `auto_ptr`, you're
  reading history.
- **Borrowing vs owning.** Pass `unique_ptr<T>` *by value* to transfer
  ownership; pass `T&` or `T*` to lend access. A function that just reads
  the object has no business in the ownership system at all.
- **`std::shared_ptr`** *(C++11)* — shared ownership through a reference-
  counted *control block*. Costs: two pointers wide, atomic count updates
  on every copy, and a second allocation — unless you use
  **`std::make_shared`**, which fuses object + control block into one.
- **`std::weak_ptr`** *(C++11)* — observes without owning; the escape
  hatch from reference cycles (two `shared_ptr`s pointing at each other
  keep each other alive forever — destructors never run). `lock()`
  yields a `shared_ptr` or null, atomically.
- **Custom deleters** — `unique_ptr` can own *any* resource with a
  release function (C handles, mmaps, sockets). A stateless functor
  deleter is free (empty-base optimization); a function-pointer deleter
  doubles the pointer's size.

## Version notes

| Feature | Standard |
|---|---|
| `unique_ptr`, `shared_ptr`, `weak_ptr` | C++11 |
| `make_shared` | C++11 |
| `make_unique` | C++14 |
| `auto_ptr` | deprecated C++11, **removed C++17** |
| `shared_ptr<T[]>` array support | C++17 |
| `make_shared` for arrays | C++20 |
