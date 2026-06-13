// layout3.cpp
//
// `new T` does two jobs: allocate memory AND construct an object in it.
// PLACEMENT NEW splits them — construct into memory you already have:
//
//     alignas(T) std::byte storage[sizeof(T)];   // raw bytes, no object
//     T* t = new (storage) T(args);              // construct HERE
//     ...
//     t->~T();                                   // YOU destroy it —
//                                                // nothing else will
//
// No allocation happens; `new (ptr)` just runs the constructor at ptr.
// This is the machinery inside std::vector (raw capacity, constructed
// prefix), std::optional (storage + maybe-an-object), arenas, object
// pools. You'll rarely write it — but you'll debug things built on it.
//
// (C++20 spells it std::construct_at(ptr, args) / std::destroy_at(ptr),
// constexpr-friendly. Same machinery.)
//
// The exercise: a fixed Slot that hosts a Session object on demand.
// Both TODOs are missing — right now nothing ever constructs or
// destroys the Session, and the counters know.
//
// Task: construct in activate() (placement new), destroy in
// deactivate() (explicit destructor call).

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
        // TODO: construct a Session into storage_ with placement new
        return nullptr;
    }

    void deactivate(Session* s) {
        // TODO: run the destructor — nobody else will
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
    assert(Session::alive == 0);          // destructor ran, string freed

    // The slot is reusable — that's the point of the pattern:
    Session* t = slot.activate("bob");
    assert(Session::alive == 1 && t->user == "bob");
    slot.deactivate(t);
    assert(Session::alive == 0);
    return 0;
}

// I AM NOT DONE
