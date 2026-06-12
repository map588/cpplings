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
// Task: RingBuffer below takes its config the runtime way and lost all
// of those properties. Make `Cfg` a class-type NTTP; the asserts and
// the static_asserts encode the contract.

// I AM NOT DONE

#include <cassert>
#include <cstddef>
#include <type_traits>

struct RingConfig {
    std::size_t capacity;
    bool overwrite_when_full;
};

class RingBuffer {                    // TODO: template <RingConfig Cfg>
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
