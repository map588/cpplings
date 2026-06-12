// friend1.cpp — solution

#include <cassert>
#include <sstream>

class Temperature {
public:
    explicit Temperature(double celsius) : celsius_(celsius) {}

    // Hidden friends: full access, findable only through ADL.
    friend bool is_freezing(const Temperature& t) {
        return t.celsius_ <= 0.0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Temperature& t) {
        return os << t.celsius_ << "°C";
    }

private:
    double celsius_;
};

int main() {
    Temperature mild(21.5);
    Temperature cold(-4.0);

    assert(!is_freezing(mild));
    assert(is_freezing(cold));

    std::ostringstream out;
    out << mild;
    assert(out.str() == "21.5°C");
    return 0;
}
