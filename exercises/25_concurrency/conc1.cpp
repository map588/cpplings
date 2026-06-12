// conc1.cpp
//
// A std::thread (C++11) starts running at construction. Before it is
// destroyed you MUST decide its fate:
//
//     t.join();      // wait for it to finish
//     t.detach();    // let it run free (and lose all control — rare)
//
// Destroy one still "joinable" and the runtime calls std::terminate —
// run this file and watch. Why so harsh? Because both defaults are
// disasters: implicit join can deadlock; implicit detach leaves a
// thread using objects that are being destroyed. C++ refuses to pick.
//
// C++20's std::jthread picks SAFELY: it joins in its destructor — RAII
// applied to threads (it also carries a stop_token for cooperative
// cancellation). Default to jthread.
//
// Task: fix word_total — either join the threads before returning, or
// (better) make them jthreads and let scope do it. Note the result
// pattern: each thread writes ITS OWN slot, no sharing, no lock needed
// — the parallel-sum shape you'll use everywhere.

// I AM NOT DONE

#include <cassert>
#include <string>
#include <thread>
#include <vector>

int count_words(const std::string& text) {
    int n = 0;
    bool in_word = false;
    for (char c : text) {
        bool space = (c == ' ');
        if (!space && !in_word) ++n;
        in_word = !space;
    }
    return n;
}

int word_total(const std::vector<std::string>& docs) {
    std::vector<int> per_doc(docs.size());

    std::vector<std::thread> workers;
    for (std::size_t i = 0; i < docs.size(); ++i) {
        workers.emplace_back([&docs, &per_doc, i] {
            per_doc[i] = count_words(docs[i]);   // own slot: race-free
        });
    }
    // ...and the threads? Still running as `workers` is destroyed.

    int total = 0;
    for (int n : per_doc) total += n;
    return total;
}

int main() {
    std::vector<std::string> docs = {
        "the quick brown fox",
        "jumps over",
        "the lazy dog",
    };
    assert(word_total(docs) == 9);
    return 0;
}
