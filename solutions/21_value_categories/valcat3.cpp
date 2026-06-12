// valcat3.cpp — solution

#include <cassert>
#include <string>

struct Reading {
    std::string sensor_name;
    double value;
};

Reading make_reading() {
    return {"thermocouple-array-7-bridge-northeast", 36.6};
}

const std::string& pick_name(const Reading& r) {
    return r.sensor_name;
}

int main() {
    const Reading& whole = make_reading();
    assert(whole.value == 36.6);

    // Bind the member SUBOBJECT directly: the compiler sees the
    // temporary in this very initialization and extends ALL of it.
    const std::string& name = make_reading().sensor_name;
    assert(name == "thermocouple-array-7-bridge-northeast");
    return 0;
}
