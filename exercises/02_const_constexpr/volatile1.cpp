// volatile1.cpp
//
// `volatile` (inherited from C) is the OTHER type qualifier — it rides the
// exact same rails as const. Where const promises "no writes through this
// name", volatile tells the compiler: every read and write of this object
// written in the source is a REAL access with effects the compiler cannot
// see. Each one must be emitted, in order — no caching the value in a
// register, no folding two loads into one, no deleting a "useless" store.
//
// Why would a mere load matter? Memory-mapped I/O: a device register looks
// like memory, but reading STATUS may pop a hardware FIFO, and the value
// can change between two reads because the DEVICE changes it. Without
// volatile, the optimizer — seeing no C++ code modify the variable — would
// happily hoist the load out of `while (!(*reg & DONE))` and spin forever
// on one stale read. The other legitimate use: a `volatile
// std::sig_atomic_t` flag set from a signal handler.
//
// What volatile is NOT — and hear this loudly — is a thread-synchronization
// tool. It does not make accesses atomic, does not order them against other
// memory, does not publish data to other cores. Java's and C#'s volatile
// do; C++'s does NOT. Threads need std::atomic (C++11) — module 25.
//
// Because it's a qualifier, the const rules you already know apply
// verbatim: T* converts to volatile T* for free, but DROPPING the
// qualifier silently is forbidden — the compiler refuses, exactly as it
// refuses to pass const data to a function taking plain T&. And the fix is
// the same one const-correctness taught you: propagate the qualifier
// through every helper that touches the data.
//
// Below, a fake device register (a plain variable standing in for real
// hardware, so the program can run) is accessed through REG — read its
// type right-to-left, const2-style: const pointer to volatile uint32_t.
// The helpers were written as if the data were ordinary memory. The calls
// don't compile.
//
// Task: make the program compile, run, and pass every assert.
//   - compiles warning-free; all asserts pass; exits 0
// Constraints:
//   - REG keeps its exact type — the register IS volatile
//   - no casts anywhere: don't launder the qualifier away, propagate it
//   - main() stays as-is; the "simulation" line is the device's job, don't
//     add more direct access to fake_register
//
// Note: C++20 deprecated compound assignment on volatile lvalues
// (`*REG |= x` now warns) — write the load and the store separately.


#include <cassert>
#include <cstdint>

// In real firmware REG would be born from the datasheet, e.g.
//   auto* const REG = reinterpret_cast<volatile std::uint32_t*>(0x4002'1018);
namespace {
std::uint32_t fake_register = 0;  // stand-in for the hardware
}
volatile std::uint32_t* const REG = &fake_register;

constexpr std::uint32_t CMD_START   = 1u << 0;  // write: start a conversion
constexpr std::uint32_t STATUS_DONE = 1u << 1;  // read: conversion finished

// --- helpers: every register access in the codebase goes through these ---

void write_reg(std::uint32_t* reg, std::uint32_t value) {
    *reg = value;
}

std::uint32_t read_reg(const std::uint32_t* reg) {
    return *reg;
}

// Spin until every bit of `mask` reads as set; return the last value read.
// On a volatile pointer the compiler must re-load *reg every pass — which
// is the entire point of the loop.
std::uint32_t wait_until_set(const std::uint32_t* reg, std::uint32_t mask) {
    std::uint32_t v = *reg;
    while ((v & mask) != mask) {
        v = *reg;
    }
    return v;
}

int main() {
    write_reg(REG, CMD_START);           // kick off a conversion
    assert(fake_register == CMD_START);  // the store really happened

    fake_register |= STATUS_DONE;        // (simulation: the device finishes)

    std::uint32_t status = wait_until_set(REG, STATUS_DONE);
    assert(status == (CMD_START | STATUS_DONE));
    assert(read_reg(REG) == status);

    return 0;
}
// I AM NOT DONE
