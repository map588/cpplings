// extras4.cpp — solution

#include <cassert>
#include <coroutine>
#include <utility>
#include <vector>

// ----- minimal generator machinery (C++23 spells it std::generator) ----
template <typename T>
class Generator {
public:
    struct promise_type {
        T current;
        Generator get_return_object() {
            return Generator{Handle::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = v;
            return {};
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
    co_yield n;
    while (n != 1) {
        n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
        co_yield n;                  // suspend here; `n` sleeps in the frame
    }
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
