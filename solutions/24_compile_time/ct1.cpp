// ct1.cpp — solution

#include <cassert>
#include <cstring>
#include <type_traits>
#include <vector>

std::vector<unsigned char> disk;

template <typename T>
void write_binary(const T& obj) {
    static_assert(std::is_trivially_copyable_v<T>,
                  "T has remote parts (heap members?) — memcpy would "
                  "serialize dangling pointers. Write a real serializer.");
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
    char player[16];           // bytes ARE the value: memcpy-safe
    int level;
    double health;
};

int main() {
    SaveGame s{"ada", 7, 99.5};
    write_binary(s);

    SaveGame restored = read_binary<SaveGame>();
    assert(restored.level == 7 && restored.health == 99.5);
    assert(std::strcmp(restored.player, "ada") == 0);

    static_assert(std::is_trivially_copyable_v<SaveGame>);
    return 0;
}
