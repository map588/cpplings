// asm1.cpp
//
// The last resort — and the one inline asm idiom you'll actually
// use. GCC/clang extended asm has the shape:
//
//     asm("template with %0, %1, ..."
//         : outputs      // "=r"(var)  →  %0, %1, ... in order
//         : inputs       // "r"(expr)
//         : clobbers);   // "memory", condition codes, scratch regs
//
// The constraint strings are a contract with the REGISTER ALLOCATOR:
//     "r"   put this INPUT in any register
//     "=r"  this is an OUTPUT: written, register, previous value dead
//     "=&r" output, EARLY-CLOBBER: written BEFORE the last input is
//           read — allocator must not share a register with any input
//     "memory" (clobber): "I may have touched any memory" — a
//           compiler-level fence (no reordering of loads/stores past
//           it; the CPU is a separate matter, module 25)
//
// Get a constraint wrong and the bug is silent register corruption —
// found only by reading disassembly. Which is why the idiom worth
// memorizing is the asm that contains NO INSTRUCTIONS AT ALL:
//
//     asm volatile("" : "+r"(x));            // "DoNotOptimize(x)"
//     asm volatile("" ::: "memory");         // "ClobberMemory()"
//
// An empty template — zero machine code — but the CONSTRAINTS still
// bind: the compiler must materialize x in a register at this point
// ("+r": read AND written, so it can't assume the value it knew),
// and must not cache memory across the fence. Every benchmark
// harness (google/benchmark et al.) is built on these two lines:
// without them, -O2 sees your timing loop computes an unused value,
// deletes it, and reports that your function takes 0 ns.
//
// keep() below is that idiom, ready to use. Your job is the real
// asm: one add, both architectures. The aarch64 `add` is a single
// three-operand instruction — output and inputs coexist. The x86_64
// path needs TWO instructions (mov, then addl) — it writes %0
// before it has read %2, so its output constraint needs the
// early-clobber &.
//
// Task: fix the output constraints (the compiler rejects the file
// as shipped — read its complaint).
//   - every assert passes
// Constraints (pun intended):
//   - the asm templates are correct — only the constraint strings
//     after the ':' need work
//   - keep the x86 path's & story in mind: "=r" there compiles, and
//     then MAY corrupt a and b's register — the assert can't even
//     promise to catch it. That's what "silent" means.

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
        : "r"(out)                    // TODO: `out` is an OUTPUT
        : "r"(a), "r"(b));
#elif defined(__x86_64__) || defined(_M_X64)
    // two instructions: out ← a, then out += b — early-clobber!
    asm("movl %1, %0\n\t"
        "addl %2, %0"
        : "r"(out)                    // TODO: output, and written EARLY
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

// I AM NOT DONE
