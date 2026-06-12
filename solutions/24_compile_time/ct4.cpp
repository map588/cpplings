// ct4.cpp — solution

#include <cassert>
#include <cstddef>
#include <type_traits>

struct RingConfig {
    std::size_t capacity;
    bool overwrite_when_full;
};

template <RingConfig Cfg>
class RingBuffer {
public:
    static_assert(Cfg.capacity > 0, "a zero-capacity ring is a typo");

    bool push(int v) {
        if (count_ == Cfg.capacity) {
            if (!Cfg.overwrite_when_full) return false;
            tail_ = (tail_ + 1) % Cfg.capacity;
            --count_;
        }
        data_[(tail_ + count_) % Cfg.capacity] = v;
        ++count_;
        return true;
    }

    std::size_t count() const { return count_; }

private:
    std::size_t tail_ = 0;
    std::size_t count_ = 0;
    int data_[Cfg.capacity];          // exactly sized — no 64-slot hope
};

int main() {
    RingBuffer<RingConfig{.capacity = 4, .overwrite_when_full = false}> strict;
    assert(strict.push(1) && strict.push(2) && strict.push(3) && strict.push(4));
    assert(!strict.push(5));
    assert(strict.count() == 4);

    RingBuffer<RingConfig{.capacity = 2, .overwrite_when_full = true}> rolling;
    assert(rolling.push(1) && rolling.push(2) && rolling.push(3));
    assert(rolling.count() == 2);

    static_assert(!std::is_same_v<decltype(strict), decltype(rolling)>);
    return 0;
}
