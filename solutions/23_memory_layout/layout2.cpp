// layout2.cpp — solution

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <memory>

float dsp_sum4(const float* lane) {
    assert(reinterpret_cast<std::uintptr_t>(lane) % 16 == 0 &&
           "dsp_sum4 requires a 16-byte-aligned pointer");
    return lane[0] + lane[1] + lane[2] + lane[3];
}

struct Packet {
    char tag;
    alignas(16) float lane[4];   // offset 16 now; Packet itself becomes
};                               // 16-aligned, and new honors it (C++17)

struct alignas(64) CacheLineSlot {
    long counter;
};

int main() {
    auto p = std::make_unique<Packet>();
    p->lane[0] = 1.5f; p->lane[1] = 2.5f; p->lane[2] = 3.0f; p->lane[3] = 4.0f;

    assert(dsp_sum4(p->lane) == 11.0f);

    static_assert(alignof(CacheLineSlot) == 64);
    static_assert(sizeof(CacheLineSlot) == 64);   // padded out to a full line
    return 0;
}
