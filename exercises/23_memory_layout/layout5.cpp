// layout5.cpp  — module capstone: COUNT THE ALLOCATIONS
//
// You can replace global operator new — and counting calls to it is
// the cheapest profiler you'll ever own. The override below forwards
// to malloc and tallies. Suddenly this course's claims are up for
// audit as measurable fact:
//
//   - module 15 claimed small strings and big strings pay differently
//     (SSO)
//   - module 10 claimed make_shared FUSES what shared_ptr<T>(new T)
//     buys separately — recall where a control block lives
//   - module 16 claimed a vector growing from nothing pays once per
//     capacity step (doubling from 1 on this course's platforms), and
//     that a caller who knows the final size can pre-plan instead
//
// Predict every TODO from those claims — before running anything.
//
// Then the finale: make_report() allocates more than it must. It
// knows k before the loop starts — let it pre-plan the storage, and
// the final assert's 1 becomes the truth.
//
// Task: replace each TODO with an allocation count, then make
// make_report cost exactly one allocation.
//   - every assert passes
// Constraints:
//   - don't touch the operator new/delete machinery
//   - make_report keeps its push_back loop — the fix is one added
//     line, not a rewrite
//   - the final assert keeps its 1

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
    assert(news == TODO);                       // the two-step: what does
                                                // each step buy?

    news = 0;
    { auto fused = std::make_shared<int>(5); }
    assert(news == TODO);

    news = 0;
    { auto report = make_report(8); }
    assert(news == 1);     // shipped, it's 4 (the 1→2→4→8 growth chain).
    return 0;              // make_report knows k — make it ONE allocation.
}

// I AM NOT DONE
