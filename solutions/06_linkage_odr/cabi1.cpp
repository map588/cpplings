// cabi1.cpp — solution

#include <cassert>

// extern "C" gives these declarations C language linkage: the compiler now
// looks up the plain, unmangled symbols the vendor actually emitted.
extern "C" {
int vendor_init(void);
int vendor_read_raw(void);
int vendor_to_millicelsius(int raw);
}

int main() {
    assert(vendor_init() == 0);  // 0 = success, C style

    int raw = vendor_read_raw();
    assert(raw == 987);          // raw ADC counts

    assert(vendor_to_millicelsius(raw) == 24675);  // 24.675 °C — balmy
    return 0;
}
