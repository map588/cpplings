// constexpr1.cpp — solution

#include <cassert>
#include <cstddef>

// constexpr: callable at compile time. Loops/ifs/locals in the body are
// fine since C++14.
constexpr int max_packet_size(int header_bytes, int payload_bytes) {
    int total = header_bytes + payload_bytes;
    if (total % 8 != 0) {
        total += 8 - total % 8;
    }
    return total;
}

int main() {
    constexpr int packet_size = max_packet_size(11, 32);
    static_assert(packet_size == 48, "11+32=43, rounded up to 48");

    unsigned char buffer[max_packet_size(11, 32)] = {};
    assert(sizeof(buffer) == 48);

    int runtime_header = 20;
    assert(max_packet_size(runtime_header, 100) == 120);
    return 0;
}
