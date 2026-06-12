// conc4.cpp — solution

#include <cassert>
#include <chrono>
#include <mutex>
#include <thread>

struct Account {
    int balance = 0;
    std::mutex m;
};

void transfer(Account& from, Account& to, int amount) {
    // Both mutexes, one acquisition, deadlock-avoidance built in:
    std::scoped_lock lk(from.m, to.m);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    from.balance -= amount;
    to.balance += amount;
}

int main() {
    Account alice{.balance = 100};
    Account bob{.balance = 100};

    {
        std::jthread t1(transfer, std::ref(alice), std::ref(bob), 30);
        std::jthread t2(transfer, std::ref(bob), std::ref(alice), 10);
    }

    assert(alice.balance == 80);
    assert(bob.balance == 120);
    return 0;
}
