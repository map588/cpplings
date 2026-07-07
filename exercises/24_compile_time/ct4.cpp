// ct4.cpp
//
// Module 12 used VALUES as template parameters (array sizes). C++20
// extended NTTPs to CLASS TYPES — a whole config struct as a template
// argument:
//
//     struct PoolConfig {
//         std::size_t slot_size;
//         std::size_t slots;
//         bool zero_on_free;
//     };
//
//     template <PoolConfig Cfg>            // a STRUCT VALUE, not a type
//     class Pool {
//         std::byte storage_[Cfg.slot_size * Cfg.slots];
//         static_assert(Cfg.slot_size % 8 == 0, "keep slots aligned");
//     };
//
//     Pool<PoolConfig{.slot_size = 64, .slots = 16}> small_pool;
//
// What you get over constructor parameters: sizes become compile-time
// constants (real arrays, no heap), invalid configs are compile errors
// inside the template, and DIFFERENT CONFIGS ARE DIFFERENT TYPES — you
// cannot hand a Pool<tiny> to code expecting Pool<huge>. The config is
// part of the type's identity. (Requirement: the NTTP type must be a
// structural type — public members, no custom ==, roughly.)
//
// RingBuffer below takes its config the runtime way and lost all of
// those properties — note data_'s wishful 64.
//
// Task: rebuild RingBuffer so its config is a compile-time property
// of the type.
//   - main compiles AS WRITTEN — RingBuffer<RingConfig{...}> is the
//     interface the callers already expect
//   - data_ has EXACTLY capacity elements: no 64-slot hope, no heap
//   - every assert passes, including the different-types static_assert
// Constraints:
//   - RingConfig itself stays unchanged
//   - push()'s logic stays as-is apart from where the config comes from
//   - don't change main

#include <cassert>
#include <cstddef>
#include <type_traits>

struct RingConfig {
    std::size_t capacity;
    bool overwrite_when_full;
};

class RingBuffer {                    // TODO: the config arrives one whole
                                      // compile phase too late
public:
    explicit RingBuffer(RingConfig cfg) : cfg_(cfg) {}

    bool push(int v) {
        if (count_ == cfg_.capacity) {
            if (!cfg_.overwrite_when_full) return false;
            tail_ = (tail_ + 1) % cfg_.capacity;
            --count_;
        }
        data_[(tail_ + count_) % cfg_.capacity] = v;
        ++count_;
        return true;
    }

    std::size_t count() const { return count_; }

private:
    RingConfig cfg_;
    std::size_t tail_ = 0;
    std::size_t count_ = 0;
    int data_[64];                    // hopes capacity <= 64...
};

int main() {
    RingBuffer<RingConfig{.capacity = 4, .overwrite_when_full = false}> strict;
    assert(strict.push(1) && strict.push(2) && strict.push(3) && strict.push(4));
    assert(!strict.push(5));                  // full and strict: refused
    assert(strict.count() == 4);

    RingBuffer<RingConfig{.capacity = 2, .overwrite_when_full = true}> rolling;
    assert(rolling.push(1) && rolling.push(2) && rolling.push(3));
    assert(rolling.count() == 2);             // 1 was overwritten

    // Different configs are DIFFERENT TYPES:
    static_assert(!std::is_same_v<decltype(strict), decltype(rolling)>);
    return 0;
}

// I AM NOT DONE
