// layout3.cpp — solution

#include <cassert>
#include <cstddef>
#include <new>
#include <string>

struct Session {
    inline static int alive = 0;

    std::string user;

    explicit Session(std::string u) : user(std::move(u)) { ++alive; }
    ~Session() { --alive; }
};

class Slot {
public:
    Session* activate(const std::string& user) {
        return new (storage_) Session(user);   // construct, don't allocate
    }

    void deactivate(Session* s) {
        s->~Session();                          // destroy, don't deallocate
    }

private:
    alignas(Session) std::byte storage_[sizeof(Session)];
};

int main() {
    Slot slot;

    Session* s = slot.activate("ada");
    assert(Session::alive == 1);
    assert(s->user == "ada");

    slot.deactivate(s);
    assert(Session::alive == 0);

    Session* t = slot.activate("bob");
    assert(Session::alive == 1 && t->user == "bob");
    slot.deactivate(t);
    assert(Session::alive == 0);
    return 0;
}
