# cpplings curriculum

Six parts, 27 modules, **148 exercises — all written**. Everything compiles as
**C++20**; tags like *(C++17)* mark when a feature entered (or left) the
language, since knowing the timeline is half of reading real-world code.

Status: ✅ written — the curriculum is complete.

---

## Part I — Foundations

| Module | Status | Covers |
|---|---|---|
| `00_intro` (3) | ✅ | Compilation model (preprocess → compile → assemble → link), the exercise workflow, `assert` / `static_assert` *(C++11; message-less form C++17)* as the feedback loop |
| `01_types` (6) | ✅ | Initialization forms (`=` vs `()` vs `{}` *(C++11)*) and narrowing; `auto` deduction rules *(C++11)*; integral promotion; signed/unsigned conversion traps, `std::ssize`/`std::cmp_less` *(C++20)*; fixed-width types *(C++11)* |
| `02_const_constexpr` (5) | ✅ | const correctness; const & pointers; const member functions, `mutable`; `constexpr` *(C++11, relaxed C++14)*; `consteval`/`constinit` *(C++20)* |
| `03_references_pointers` (5) | ✅ | References as aliases; binding rules & lifetime extension; parameter-type design; `nullptr` *(C++11)*; ownership bugs under ASan |
| `04_functions` (5) | ✅ | Overload resolution ranking & ambiguity; reference overloads (`&`/`const&`/`&&`) as the basis of move semantics; default arguments (trailing rule, per-call evaluation); `static` locals & magic statics *(C++11)* |
| `05_namespaces_using` (6) | ✅ | Namespaces & nesting *(compact `a::b::c` syntax C++17)*; the four faces of `using`: using-declaration vs using-directive (why `using namespace std;` belongs nowhere near a header), type aliases & alias templates *(C++11, the modern `typedef`)*, `using enum` *(C++20)*, inheriting constructors `using Base::Base` *(C++11)*; ADL — why `std::cout << x` finds the right `operator<<`, and the `swap` two-step; anonymous namespaces; `inline namespace` versioning note |
| `06_linkage_odr` (5) | ✅ | Translation units; headers & include guards; internal vs external linkage; `extern`; ODR violations; `inline` variables *(C++17)* |

## Part II — Classes & ownership

| Module | Status | Covers |
|---|---|---|
| `07_classes1` (6) | ✅ | Member-init lists (const/ref members, declaration-order rule); NSDMI & delegating ctors *(C++11)*; `explicit` & implicit-conversion bugs; destruction order; `=default`/`=delete` *(C++11)*; aggregates & designated initializers *(C++20)* |
| `08_classes2` (7) | ✅ | Rule of 3 (deep copy by hand, once) and rule of zero; the six special members & suppression rules (a user dtor silently kills moves — measured); operator overloading & free-function symmetry; `operator<=>` *(C++20)*; hidden-friend idiom; `inline static` members *(C++17)* |
| `09_move_semantics` (7) | ✅ | `std::move` is a cast (buffer-identity proof); rule of 5 (steal + null + `noexcept`); `move_if_noexcept` & vector growth; guaranteed elision *(C++17)* and the `return std::move` pessimization; sink parameters; forwarding refs & `std::forward`; the const-blocks-move trap |
| `10_raii_smart_ptrs` (6) | ✅ | RAII guards (early-return leak made measurable); `unique_ptr` + `make_unique` *(C++14)* zero-overhead proof; move-only ownership transfer vs borrowing (`T&`); `shared_ptr` control-block costs & `use_count`; `weak_ptr` breaking cycles (`lock()`); custom deleters for C APIs (functor vs function-pointer size) |
| `11_polymorphism` (6) | ✅ | `virtual` dispatch through refs only; `override` *(C++11)* catching hide-not-override; virtual destructors (skipped-dtor proof via counter); pure virtuals turning missing impls into compile errors; object slicing (by-value params & `vector<Base>` → `vector<unique_ptr<Base>>`); vptr size cost + `final` devirtualization quiz |

## Part III — Generic programming

| Module | Status | Covers |
|---|---|---|
| `12_templates1` (6) | ✅ | Function/class templates & deduction (incl. the mixed-type conflict); dependent names & `typename`; full specialization; reading instantiation-chain errors (a designed wall-of-noise exercise); NTTPs & the array-decay `countof` bug |
| `13_lambdas` (6) | ✅ | Lambdas from zero *(C++11)*: desugared to a functor first; value vs ref captures (creation-time rule); init-captures + `mutable` & copied closure state *(C++14)*; dangling `[&]` through vector reallocation (ASan); algorithms + generic lambdas; closure sizeof, function-pointer conversion, `std::function` type erasure |
| `14_templates2` (7) | ✅ | Concepts *(C++20)*: composing std concepts, `requires` expressions, constrained overloading replacing SFINAE; variadics & folds *(C++17)* incl. comma-fold `join()`; CTAD + deduction guides *(C++17)*; `if constexpr` discarded branches *(C++17)* |

## Part IV — The standard library

| Module | Status | Covers |
|---|---|---|
| `15_strings_views` (4) | ✅ | SSO caught red-handed (`.data()` inside the object); `string_view` *(C++17)* zero-copy parsing; the temporary-dangling composition bug (ASan); `from_chars` *(C++17)* with full error contract |
| `16_containers` (6) | ✅ | `vector` growth invalidation (ASan) + `reserve`; erase-while-iterating idiom; `map::operator[]` inserts-on-read + `contains`/`find`; `insert`/`try_emplace` *(C++17)* semantics; `std::span` *(C++20)*; invalidation-rules quiz (node stability, rehash survival) |
| `17_algorithms` (4) | ✅ | `accumulate` (init-type trap!)/`max_element`/`find`; erase–remove idiom → `std::erase` *(C++20)*; sorted-range precondition + `lower_bound`; `transform` + `back_inserter` |
| `18_ranges` (4) | ✅ | Range algorithms + projections *(C++20)*; view laziness proven with call counters; infinite `iota` + `take`; `std::ranges::dangling` turning use-after-free into a compile error |
| `19_vocabulary_types` (6) | ✅ | `optional` *(C++17)* vs sentinels; `*` (UB) vs `.value()` (throws) vs `value_or`; `variant` + `get_if`; `std::visit` + overloaded idiom as the closed-set alternative to virtual; `tuple` + structured bindings *(C++17)*; `any` quiz + `std::expected` *(C++23)* reading note |

## Part V — Correctness

| Module | Status | Covers |
|---|---|---|
| `20_errors` (5) | ✅ | Throw by value / catch by `const&` (slicing!); RAII through unwinding (no `finally` needed); `noexcept` broken-promise → `terminate` + `noexcept()` operator; the safety-guarantee ladder & strong-guarantee recipe (build aside, commit nothrow); exceptions vs `optional` for expected outcomes |
| `21_value_categories` (5) | ✅ | The taxonomy via `decltype((expr))` (incl. `++x` vs `x++`); return type ⇒ call category (`T&` calls are assignable); exact lifetime-extension boundary (member binding extends, function passage doesn't — ASan); reference-in/reference-out contract; ref-qualified members (`const&`/`&&` overloads, the legitimate `return std::move(member_)`) |
| `22_ub_sanitizers` (6) | ✅ | UB as contract; the famous binary-search midpoint overflow (→ `std::midpoint` *(C++20)*); OOB `<=` + `.at()` for untrusted input; stack-use-after-scope; misaligned/type-punned access → `memcpy`/`bit_cast` *(C++20)*; integer div-by-zero (vs IEEE float!); shift contracts & promotion traps |

## Part VI — Under the hood

| Module | Status | Covers |
|---|---|---|
| `23_memory_layout` (5) | ✅ | Padding & member ordering (24→16 bytes); `alignas` *(C++11)* for SIMD/cache lines; placement new + explicit dtor; `[[no_unique_address]]` *(C++20)* (how module 10's deleter was free); counting allocations by replacing `operator new` (SSO, growth chains, `make_shared`'s fused allocation — audited) |
| `24_compile_time` (5) | ✅ | Trait gates (`is_trivially_copyable` guarding memcpy serialization); `consteval` lookup tables; constexpr `std::vector` scratch *(C++20)*; class-type NTTPs (config structs as template args); the `if constexpr (is_constant_evaluated())` footgun *(C++20; `if consteval` C++23)* |
| `25_concurrency` (7) | ✅ | `thread`/`jthread` joinability (`terminate` demo); data race under TSan → `atomic`; vector race → `mutex`/`lock_guard`; deterministic deadlock → `scoped_lock` *(C++17)*; lost wakeup → predicate `cv.wait`; release/acquire publish pattern; `async`/`future` parallel sum |
| `26_modern_extras` (5) | ✅ | `chrono` typed durations & `duration_cast`; `filesystem::path` decomposition quiz; `std::format` *(C++20; `std::print` C++23 note)*; a real coroutine generator (`co_yield` Collatz; `std::generator` C++23 note); graduation quiz + modules *(C++20)* reading note |

---

Each part closes with a **quiz** exercise combining its modules.

## Design rules for exercises (for contributors / future sessions)

1. One concept per exercise; one file per exercise; self-verifying via
   `assert`/`static_assert` or a sanitizer report.
2. Shipped exercises must FAIL (compile error or non-zero exit) and
   solutions must PASS — `python3 cpplings.py check-solutions` enforces both.
3. Every feature mentioned gets a standard tag the first time it appears in
   a file: `string_view (C++17)`.
4. The comment block should teach the WHY (what the compiler does
   differently), not just the syntax.
5. Styles: compile-fix for type-system topics, TODO-completion for design
   topics, quiz files to close each part.
