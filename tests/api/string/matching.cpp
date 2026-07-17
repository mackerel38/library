#include "string/matching.hpp"

int main() {
    assert(poe::wildcard_matches("abacaba", "a_a")
        == std::vector<int>({0, 2, 4}));
    assert(poe::wildcard_matches("abc", "") == std::vector<int>({0, 1, 2, 3}));
    assert(poe::wildcard_matches("a_c", "abc") == std::vector<int>({0}));
}

