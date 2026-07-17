#include <cassert>
#include <string>
#include "string/automaton.hpp"

int main() {
    poe::dfa parity{0, {{1, 0}, {0, 1}}, {1, 0}};
    assert(parity.accepts(std::string("1001"), [](char c) { return c - '0'; }));
    const auto stats = poe::accepted_substring_stats(
        parity, std::string("110"), [](char c) { return c - '0'; }
    );
    assert(stats.longest == 2 && stats.count == 3);

    const auto learned = poe::right_context_dfa(2, 1, [](const std::vector<int>& word) {
        return std::ranges::count(word, 1) % 2 == 1;
    });
    assert(learned.accepts(std::vector<int>{1, 0, 1, 1}));
    assert(!learned.accepts(std::vector<int>{1, 0, 1}));
}
