// casts1.cpp — solution

#include <cassert>
#include <cstddef>
#include <cstdint>

std::size_t legacy_length(char* text) {
    std::size_t n = 0;
    while (text[n] != '\0') ++n;
    return n;
}

// (1) A value conversion the type system knows: static_cast.
double ratio(int hits, int total) {
    return static_cast<double>(hits) / total;
}

// (2) Only the const changes: const_cast. Legitimate here because
// legacy_length never writes through the pointer.
std::size_t label_length(const char* label) {
    return legacy_length(const_cast<char*>(label));
}

// (3) Pointer → integer is a reinterpretation of bits: reinterpret_cast.
bool is_aligned(const int* p) {
    return reinterpret_cast<std::uintptr_t>(p) % alignof(int) == 0;
}

// (4) No cast at all. The C-style cast was a const_cast in disguise,
// followed by a WRITE to the caller's const object — undefined
// behavior on kLimit. "The limit plus one" never needed to modify
// anything.
int bumped(const int& limit) {
    return limit + 1;
}

const int kLimit = 10;

int main() {
    assert(ratio(1, 4) == 0.25);
    assert(label_length("gauge") == 5);

    int reading = 0;
    assert(is_aligned(&reading));

    assert(bumped(kLimit) == 11);
    assert(kLimit == 10);
    return 0;
}
