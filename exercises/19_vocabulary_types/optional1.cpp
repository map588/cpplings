// optional1.cpp
//
// find_device() returns -1 for "not found" — a SENTINEL. Sentinels have
// two chronic diseases:
//   1. nothing forces the caller to check (-1 indexes an array just
//      fine, with -1 * sizeof(T) bytes of carnage)
//   2. someday the sentinel becomes a legal value and every caller
//      breaks at once
//
// std::optional<T> (C++17) moves "might be absent" into the TYPE:
//
//     std::optional<T> find(...);                // can't be misread
//
//     if (auto v = find(...)) use(*v);           // operator bool + deref
//     find(...).value_or(fallback)               // fallback in one call
//     v.has_value()  /  v.value()                // explicit spellings
//
// The empty state is std::nullopt — `return std::nullopt;` reads
// exactly like what it means.
//
// Task: rewrite find_device so the sentinel is gone and main compiles
// as written.
//   - the program compiles and every assert passes
//   - the call sites in main define the API — read them to pin down
//     the exact return type they demand
// Constraints:
//   - do not change main
//   - no negative numbers, no magic values — absence lives in the type

#include <cassert>
#include <optional>
#include <string>
#include <vector>

// Index of the device, or ... -1. In an unsigned world. Hm.
int find_device(const std::vector<std::string>& devices,
                const std::string& name) {
    for (std::size_t i = 0; i < devices.size(); ++i) {
        if (devices[i] == name) return static_cast<int>(i);
    }
    return -1;
}

int main() {
    std::vector<std::string> devices = {"lidar", "imu", "gps"};

    auto imu = find_device(devices, "imu");
    assert(imu.has_value());
    assert(*imu == 1);

    auto radar = find_device(devices, "radar");
    assert(!radar.has_value());
    assert(radar.value_or(99) == 99);

    if (auto gps = find_device(devices, "gps")) {   // bool test + scope
        assert(*gps == 2);
    }
    return 0;
}

// I AM NOT DONE
