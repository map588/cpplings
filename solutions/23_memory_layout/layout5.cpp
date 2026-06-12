// layout5.cpp — solution

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

std::vector<int> make_report(int k) {
    std::vector<int> out;
    out.reserve(static_cast<std::size_t>(k));   // module 16: one allocation
    for (int i = 0; i < k; ++i) {
        out.push_back(i);
    }
    return out;
}

int main() {
    news = 0;
    { std::string tiny = "abc"; }
    assert(news == 0);                          // SSO: lives in the object

    news = 0;
    { std::string big(100, 'x'); }
    assert(news == 1);

    news = 0;
    { auto two_step = std::shared_ptr<int>(new int(5)); }
    assert(news == 2);                          // object + control block

    news = 0;
    { auto fused = std::make_shared<int>(5); }
    assert(news == 1);                          // fused, as advertised

    news = 0;
    { auto report = make_report(8); }
    assert(news == 1);
    return 0;
}
