// extras4.cpp
//
// A COROUTINE is a function that can SUSPEND mid-body and resume later,
// its locals preserved. Any function containing co_yield / co_await /
// co_return is one — the compiler reshapes it into a state machine on
// the heap.
//
// C++20 shipped the MECHANISM but not the library types; everyone wrote
// their own "Generator" until C++23's std::generator. The Generator
// below is that boilerplate, provided and commented — skim it once
// (promise_type is the coroutine's control block; the handle resumes
// it), then write the fun part:
//
//     Generator<int> countdown(int from) {
//         while (from > 0) co_yield from--;   // suspend, hand out a value
//     }
//
// Each next() resumes the body until the next co_yield. The state —
// `from`, the loop position — sleeps between calls. It's lambdas3's
// stateful closure idea, generalized to suspendable control flow.
//
// Task: implement collatz() — yield the WHOLE Collatz sequence of n:
// n itself first, then repeatedly (even → halve, odd → 3n+1), down to
// and including 1, then finish.
//   - collatz(6) yields exactly 6, 3, 10, 5, 16, 8, 4, 2, 1
//   - collatz(1) yields exactly one value and then reports done —
//     mind the trivial case
// Constraints:
//   - it stays a coroutine: values come out one co_yield at a time
//     as the caller pulls, not from a pre-built container
//   - don't modify the Generator machinery or main

#include <cassert>
#include <coroutine>
#include <utility>
#include <vector>

// ----- minimal generator machinery (C++23 spells it std::generator) ----
template <typename T>
class Generator {
public:
    struct promise_type {                       // the coroutine's state
        T current;
        Generator get_return_object() {
            return Generator{Handle::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }   // start paused
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {  // co_yield lands here
            current = v;
            return {};                          // ...and suspends
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(Handle h) : h_(h) {}
    ~Generator() { if (h_) h_.destroy(); }
    Generator(Generator&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&&) = delete;

    // Resume until the next co_yield; false when the body finished.
    bool next() {
        h_.resume();
        return !h_.done();
    }
    T value() const { return h_.promise().current; }

private:
    Handle h_;
};
// -----------------------------------------------------------------------

Generator<int> collatz(int n) {
    co_yield n;   // TODO: ...and the rest of the sequence, down to 1
}

int main() {
    std::vector<int> seq;
    auto gen = collatz(6);
    while (gen.next()) {
        seq.push_back(gen.value());
    }

    assert(seq == (std::vector<int>{6, 3, 10, 5, 16, 8, 4, 2, 1}));

    auto trivial = collatz(1);
    assert(trivial.next() && trivial.value() == 1);
    assert(!trivial.next());
    return 0;
}

// I AM NOT DONE
