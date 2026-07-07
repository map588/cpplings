// volatile1.cpp — solution

#include <cassert>
#include <cstdint>

namespace {
std::uint32_t fake_register = 0;  // stand-in for the hardware
}
volatile std::uint32_t* const REG = &fake_register;

constexpr std::uint32_t CMD_START   = 1u << 0;  // write: start a conversion
constexpr std::uint32_t STATUS_DONE = 1u << 1;  // read: conversion finished

// The qualifier propagates through the helpers, exactly like const.
// Readers take BOTH qualifiers: they only read (const), and every read
// must really be emitted (volatile).

void write_reg(volatile std::uint32_t* reg, std::uint32_t value) {
    *reg = value;
}

std::uint32_t read_reg(const volatile std::uint32_t* reg) {
    return *reg;
}

std::uint32_t wait_until_set(const volatile std::uint32_t* reg,
                             std::uint32_t mask) {
    std::uint32_t v = *reg;
    while ((v & mask) != mask) {
        v = *reg;  // volatile: this load is re-issued every pass
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
