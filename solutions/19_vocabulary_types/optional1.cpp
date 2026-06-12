// optional1.cpp — solution

#include <cassert>
#include <optional>
#include <string>
#include <vector>

std::optional<std::size_t> find_device(const std::vector<std::string>& devices,
                                       const std::string& name) {
    for (std::size_t i = 0; i < devices.size(); ++i) {
        if (devices[i] == name) return i;
    }
    return std::nullopt;
}

int main() {
    std::vector<std::string> devices = {"lidar", "imu", "gps"};

    auto imu = find_device(devices, "imu");
    assert(imu.has_value());
    assert(*imu == 1);

    auto radar = find_device(devices, "radar");
    assert(!radar.has_value());
    assert(radar.value_or(99) == 99);

    if (auto gps = find_device(devices, "gps")) {
        assert(*gps == 2);
    }
    return 0;
}
