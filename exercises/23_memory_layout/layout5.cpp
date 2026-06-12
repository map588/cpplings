// layout5.cpp  — module capstone: COUNT THE ALLOCATIONS
//
// You can replace global operator new — and counting calls to it is
// the cheapest profiler you'll ever own. The override below forwards
// to malloc and tallies. Suddenly library behavior is measurable fact:
//
//   - std::string under SSO (module 15): zero allocations
//   - vector::reserve: exactly one
//   - vector growth from nothing: one PER capacity step (1, 2, 4...)
//   - shared_ptr<T>(new T): TWO (object + control block) —
//     make_shared: ONE, fused (module 10's claim, now audited)
//
// Predict every TODO. The growth-chain answer assumes the doubling
// libc++/libstdc++ both use from capacity 1 (this course's platforms).
//
// Task: numbers, then one fix — make_report() allocates more than it
// must; apply module 16's cure and the final assert's demand.

// I AM NOT DONE

#include <cassert>
#include <cstdlib>
#include <new>
#include <string>
#include <memory>
#include <vector>

inline int news = 0;

void* operator new(std::size_t n) {
    ++news;
    if (void* p = std::malloc(n)) return p;
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

constexpr int TODO = -1;

std::vector<int> make_report(int k) {
    std::vector<int> out;
    for (int i = 0; i < k; ++i) {
        out.push_back(i);             // growth chain: 1, 2, 4, 8(, ...)
    }
    return out;
}

int main() {
    news = 0;
    { std::string tiny = "abc"; }
    assert(news == TODO);                       // SSO (module 15)

    news = 0;
    { std::string big(100, 'x'); }
    assert(news == TODO);

    news = 0;
    { auto two_step = std::shared_ptr<int>(new int(5)); }
    assert(news == TODO);                       // object + control block

    news = 0;
    { auto fused = std::make_shared<int>(5); }
    assert(news == TODO);

    news = 0;
    { auto report = make_report(8); }
    assert(news == 1);     // shipped, it's 4 (the 1→2→4→8 growth chain).
    return 0;              // make_report knows k — make it ONE allocation.
}
