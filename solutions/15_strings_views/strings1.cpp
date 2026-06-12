// strings1.cpp — solution

#include <cassert>
#include <string>

bool chars_inside_object(const std::string& s) {
    const char* lo = reinterpret_cast<const char*>(&s);
    const char* hi = lo + sizeof(std::string);
    return s.data() >= lo && s.data() < hi;
}

int main() {
    std::string tiny = "cat";
    std::string huge(100, 'x');

    assert(chars_inside_object(tiny) == true);     // SSO: in the object
    assert(chars_inside_object(huge) == false);    // heap buffer

    std::string empty;
    assert((empty.capacity() > 0) == true);        // the SSO buffer is free

    std::string tiny2 = std::move(tiny);
    std::string huge2 = std::move(huge);
    assert(tiny2 == "cat" && huge2.size() == 100);
    return 0;
}
