// ct1.cpp
//
// write_binary() saves any object by memcpy-ing its bytes. That's only
// CORRECT for trivially copyable types — no pointers to fix up, no
// ownership, byte-identity IS object-identity. Handed a type with a
// std::string member, it cheerfully serializes a dangling heap pointer
// to disk. Today it compiles. The corruption is at restore time.
//
// Type traits make requirements like this enforceable AT COMPILE
// TIME: one declaration inside the template turns "corrupts saves at
// 3 AM" into a compile error carrying YOUR message, at the guilty
// instantiation. (<type_traits> is full of such predicates:
// is_standard_layout for C interop, is_trivially_destructible for
// skip-the-dtors optimizations — modules 08/09 already used
// is_nothrow_move_constructible. The one this file needs is spelled
// out at the bottom of main.)
//
// Task 1: gate write_binary so any non-trivially-copyable T is
// refused at compile time, with a message that says why.
// Task 2: redesign SaveGame to pass the gate — the strcmp assert
// dictates what shape the player name must take (the classic
// file-format answer).
//   - the file compiles, every assert passes, runs clean under the
//     sanitizers
// Constraints:
//   - the gate lives inside write_binary, not at call sites
//   - keep read_binary, main, and every assert unchanged
//   - SaveGame keeps all three fields: player, level, health

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
    assert(std::strcmp(restored.player, "ada") == 0);   // this call constrains
                                                        // player's new type

    static_assert(std::is_trivially_copyable_v<SaveGame>);
    return 0;
}

// I AM NOT DONE
