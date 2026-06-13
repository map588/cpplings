// layout2.cpp
//
// Natural alignment isn't always enough. Two real customers demand MORE:
//
//   SIMD — aligned vector loads (16/32/64-byte) are the fast path, and
//   APIs documented "pointer must be N-aligned" mean it; hardware
//   faults or slow paths await the careless.
//
//   CACHE LINES — two hot variables sharing one 64-byte line ping-pong
//   between cores ("false sharing"); alignas(64) gives each its own.
//
// The tool is alignas (C++11), on a type or a member:
//
//     struct alignas(64) PerCoreCounter { long n; };
//     struct Packet { char tag; alignas(16) float lane[4]; };
//
// alignas on a MEMBER also forces padding before it (the offset must
// honor the alignment) and raises the whole struct's alignment — which
// `new` honors too (aligned operator new, C++17).
//
// Below: Packet's float lane needs 16-byte alignment for dsp_sum4 (it
// checks, like a debug-mode SIMD intrinsic). As laid out, `lane` sits
// at offset 4 — a heap Packet can NEVER satisfy the check.
//
// Task: alignas the lane, then settle the TODOs (the price of an
// alignas(64) promise).

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <memory>

// Demands what real SIMD APIs demand:
float dsp_sum4(const float* lane) {
    assert(reinterpret_cast<std::uintptr_t>(lane) % 16 == 0 &&
           "dsp_sum4 requires a 16-byte-aligned pointer");
    return lane[0] + lane[1] + lane[2] + lane[3];
}

struct Packet {
    char tag;
    float lane[4];       // offset 4 — alignof(float) asks no better
};

constexpr std::size_t TODO = 9999;

struct alignas(64) CacheLineSlot {
    long counter;        // 8 meaningful bytes...
};

int main() {
    auto p = std::make_unique<Packet>();
    p->lane[0] = 1.5f; p->lane[1] = 2.5f; p->lane[2] = 3.0f; p->lane[3] = 4.0f;

    assert(dsp_sum4(p->lane) == 11.0f);

    // The price of an alignas promise — sizeof rounds up to it:
    static_assert(alignof(CacheLineSlot) == TODO);
    static_assert(sizeof(CacheLineSlot) == TODO);   // not 8!
    return 0;
}

// I AM NOT DONE
