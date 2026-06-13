// ct1.cpp
//
// write_binary() saves any object by memcpy-ing its bytes. That's only
// CORRECT for trivially copyable types — no pointers to fix up, no
// ownership, byte-identity IS object-identity. Handed a type with a
// std::string member, it cheerfully serializes a dangling heap pointer
// to disk. Today it compiles. The corruption is at restore time.
//
// Type traits make the requirement enforceable AT COMPILE TIME:
//
//     static_assert(std::is_trivially_copyable_v<T>,
//                   "T has remote parts — write a real serializer");
//
// (<type_traits> is full of these gates: is_standard_layout for C
// interop, is_trivially_destructible for skip-the-dtors optimizations
// — and modules 08/09 already used is_nothrow_move_constructible.)
//
// Task 1: add the gate to write_binary, and watch SaveGame get refused
// with YOUR message instead of corrupting saves.
// Task 2: make SaveGame trivially copyable — the player name becomes a
// fixed char array (the classic file-format answer).

#include <cassert>
#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

std::vector<unsigned char> disk;

template <typename T>
void write_binary(const T& obj) {
    disk.resize(sizeof(T));
    std::memcpy(disk.data(), &obj, sizeof(T));
}

template <typename T>
T read_binary() {
    T obj{};
    std::memcpy(&obj, disk.data(), sizeof(T));
    return obj;
}

struct SaveGame {
    std::string player;        // heap pointer in disguise!
    int level;
    double health;
};

int main() {
    SaveGame s{"ada", 7, 99.5};
    write_binary(s);

    SaveGame restored = read_binary<SaveGame>();
    assert(restored.level == 7 && restored.health == 99.5);
    assert(std::strcmp(restored.player, "ada") == 0);   // expects char[]!

    static_assert(std::is_trivially_copyable_v<SaveGame>);
    return 0;
}

// I AM NOT DONE
