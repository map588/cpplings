// cabi1.cpp        (compiled together with cabi1_vendor.cpp)
//
// C++ allows overloading — three functions may all be spelled read() — so
// the compiler MANGLES every function's symbol name, encoding namespace and
// parameter types: vendor_read_raw() is emitted as something like
// _Z15vendor_read_rawv. C has no overloading, so a C compiler emits the
// plain name: vendor_read_raw. Same source-level name, two different
// symbols — and the linker matches symbols by exact spelling, nothing else.
//
// `extern "C"` — language linkage (C++98) — is the bridge. It tells the
// C++ compiler "this function has C linkage": declarations look UP the
// unmangled symbol, definitions EMIT it. (Consequence: extern "C"
// functions can't be overloaded — there's only one flat name to go
// around.) That is why every serious C header ends in the dance
//
//   #ifdef __cplusplus
//   extern "C" {
//   #endif
//     ...declarations...
//   #ifdef __cplusplus
//   }
//   #endif
//
// — one header, plain C for a C compiler, extern "C" for a C++ one. On
// embedded targets this boundary is daily bread: the vendor HAL/SDK is C,
// your application is C++, and extern "C" is where they meet.
//
// Scenario: cabi1_vendor.cpp is a vendor's temperature-sensor SDK. It
// mirrors a header shipped for C firmware, so its functions are compiled
// with C linkage. This file transcribed the vendor's declarations as
// ordinary C++ ones — so the linker hunts for mangled symbols the vendor
// never emitted. Both TUs COMPILE fine; the failure comes from the LINKER:
// undefined reference. Learn to read that error — no line numbers, just a
// symbol nobody defined.
//
// Task: fix the declarations in THIS file so the program links, runs, and
// exits 0.
//   - all three asserts pass
// Constraints:
//   - cabi1_vendor.cpp is the vendor's — don't touch it, and don't
//     reimplement its functions here
//   - the fix goes on the declarations, not the call sites


#include <cassert>

// The vendor's header, transcribed by hand. Something about how these
// three names reach the linker is missing...
int vendor_init(void);
int vendor_read_raw(void);
int vendor_to_millicelsius(int raw);

int main() {
    assert(vendor_init() == 0);  // 0 = success, C style

    int raw = vendor_read_raw();
    assert(raw == 987);          // raw ADC counts

    assert(vendor_to_millicelsius(raw) == 24675);  // 24.675 °C — balmy
    return 0;
}
// I AM NOT DONE
