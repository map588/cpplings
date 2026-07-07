// virtdtor1.cpp
//
// What happens when an object dies through a pointer to its base class?
//
//     std::unique_ptr<Sensor> s = std::make_unique<TempSensor>();
//     // s's destructor runs:  delete (Sensor*)ptr;
//
// If ~Sensor() is NOT virtual, that delete is UNDEFINED BEHAVIOR
// ([expr.delete]: the static and dynamic types must match unless the
// base destructor is virtual). What you observe in practice: only
// ~Sensor() runs. TempSensor's members are never destroyed — its
// std::vector below never frees, its calibration counter never
// decrements. The assert catches it; on some platforms ASan also flags
// the mismatched deallocation size.
//
// The rule (memorize it):
//   a class with ANY virtual function gets  virtual ~Base() = default;
//   — you've already paid for the vtable; the virtual dtor is free.
//
// (Flip side: don't make destructors virtual on non-polymorphic classes;
// that's +8 bytes per object for nothing. And a base meant purely for
// non-owning interfaces can instead make its dtor `protected` and
// non-virtual — deletion through the base then doesn't compile.)
//
// Task: make destruction through the base pointer run the whole object's
// destructor chain.
//   - all asserts pass, including live_buffers == 0 after the scope
//   - runs clean under ASan/UBSan (the watcher builds with them)
// Constraints:
//   - main() stays as written — deleting through unique_ptr<Sensor> is
//     the point, not the bug
//   - TempSensor stays as written
//   - the fix is one line, in Sensor

#include <cassert>
#include <memory>
#include <vector>

struct Sensor {
    virtual double read() const { return 0.0; }
    ~Sensor() = default;               // so close, and yet
};

struct TempSensor : Sensor {
    inline static int live_buffers = 0;

    std::vector<double> samples;

    TempSensor() : samples(1024, 20.0) { ++live_buffers; }
    ~TempSensor() { --live_buffers; }

    double read() const override { return samples.back(); }
};

int main() {
    {
        std::unique_ptr<Sensor> s = std::make_unique<TempSensor>();
        assert(s->read() == 20.0);
        assert(TempSensor::live_buffers == 1);
    }   // deleted through Sensor* …

    assert(TempSensor::live_buffers == 0);   // …did ~TempSensor run?
    return 0;
}

// I AM NOT DONE
