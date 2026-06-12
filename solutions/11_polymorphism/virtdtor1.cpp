// virtdtor1.cpp — solution

#include <cassert>
#include <memory>
#include <vector>

struct Sensor {
    virtual double read() const { return 0.0; }
    virtual ~Sensor() = default;       // dispatch the destruction too
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
    }   // virtual dtor: ~TempSensor first, then ~Sensor

    assert(TempSensor::live_buffers == 0);
    return 0;
}
