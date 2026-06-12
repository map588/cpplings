# 25_concurrency — Threads, races, and the memory model

A **data race** — two threads touching the same memory, at least one
writing, with no synchronization — is *undefined behavior*, not "one of
them wins". This module's exercises compile with **ThreadSanitizer**
(`-fsanitize=thread`; ASan and TSan can't combine, so the runner swaps —
see each exercise's `sanitizers` entry), which catches races as they
happen with both stack traces.

## What this module covers

- **`std::thread`** *(C++11)* — destroying one still joinable calls
  `std::terminate` (joining and detaching are both plausible, so the
  language refuses to guess). **`std::jthread`** *(C++20)* joins in its
  destructor — RAII for threads; prefer it.
- **Data races & `std::atomic`** — `++counter` is a read-modify-write;
  two threads interleave it and updates vanish. `std::atomic<int>` makes
  the RMW indivisible. Atomics fix *single-variable* races only.
- **`std::mutex` + RAII locks** — for multi-step invariants and
  containers. `lock_guard` (C++11), `scoped_lock` (C++17 — takes
  *several* mutexes, deadlock-free), `unique_lock` (movable, for cv).
- **Deadlock** — two locks, opposite orders, mutual waiting, forever.
  Fixes: one lock order everywhere, or `scoped_lock(m1, m2)`.
- **`std::condition_variable`** — sleep until a predicate holds. The
  iron rule: **always wait with a predicate** (`cv.wait(lk, pred)`) —
  it survives both *spurious wakeups* and the *lost wakeup* where
  `notify` fired before you started waiting.
- **The memory model** *(C++11)* — atomics also order non-atomic memory:
  a `release` store publishes everything written before it to the
  thread that `acquire`-loads it (happens-before). `seq_cst` (the
  default) is acquire/release plus a single global order — start there;
  weaker orders are a measured optimization, not a default.
- **`std::async`/`std::future`** — task-level parallelism without
  manual threads; a future from `async` blocks in its destructor
  (infamously).

## Version notes

| Feature | Standard |
|---|---|
| `thread`, `mutex`, `condition_variable`, `atomic`, memory model, `async`/`future` | C++11 |
| `scoped_lock`, `shared_mutex` | C++17 |
| `jthread` + `stop_token`, `atomic::wait/notify`, `counting_semaphore`, `barrier`/`latch` | C++20 |
