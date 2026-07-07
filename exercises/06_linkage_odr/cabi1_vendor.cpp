// cabi1_vendor.cpp — the vendor's sensor SDK implementation. DO NOT EDIT:
// this file belongs to the vendor. It mirrors a header shipped for C
// firmware, so every entry point is compiled with C language linkage —
// plain, unmangled symbol names, reachable from C and C++ alike.

namespace {
int g_initialized = 0;  // internal state, private to this TU
}

extern "C" {

int vendor_init(void) {
    g_initialized = 1;
    return 0;  // 0 = success, like most C APIs
}

int vendor_read_raw(void) {
    return g_initialized ? 987 : -1;  // raw ADC counts (-1: not initialized)
}

int vendor_to_millicelsius(int raw) {
    return raw * 25;  // this part reads 25 millidegrees C per count
}

}  // extern "C"
