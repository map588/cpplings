// asm1.cpp — SOLUTION
//
// aarch64: "=r" — the single add instruction reads both inputs and
// writes the output at once, so the allocator may even reuse an
// input's register for out. x86_64: "=&r" — the mov writes out
// BEFORE addl reads %2; without the early-clobber & the allocator
// may put b in the same register the mov just overwrote. Silent
// corruption, no diagnostic — the & is the fix.

#include <cassert>

// The benchmark-harness idiom: force `v` to exist in a register
// here, whatever the optimizer thinks it knows about it.
inline void keep(int& v) {
    asm volatile("" : "+r"(v));
}

int asm_add(int a, int b) {
    int out;
#if defined(__aarch64__)
    // one instruction: out ← a + b   (%w0 = 32-bit view of reg %0)
    asm("add %w0, %w1, %w2"
        : "=r"(out)
        : "r"(a), "r"(b));
#elif defined(__x86_64__) || defined(_M_X64)
    // two instructions: out ← a, then out += b — early-clobber!
    asm("movl %1, %0\n\t"
        "addl %2, %0"
        : "=&r"(out)
        : "r"(a), "r"(b));
#else
#error "asm1 has aarch64 and x86_64 paths only (exercises.toml skips others)"
#endif
    return out;
}

int main() {
    assert(asm_add(20, 22) == 42);
    assert(asm_add(-5, 5) == 0);

    // The empty-asm barrier changes NOTHING about values — only what
    // the optimizer may assume about them.
    int x = 41;
    keep(x);
    assert(x == 41);
    return 0;
}
