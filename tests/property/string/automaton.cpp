#include <cassert>
#include <random>
#include <vector>
#include "dp/partition.hpp"
#include "string/automaton.hpp"

int main() {
    std::mt19937 random(418);
    for (int states = 1; states <= 7; ++states) {
        for (int trial = 0; trial < 300; ++trial) {
            poe::dfa automaton;
            automaton.next.assign(states, std::vector<int>(2));
            automaton.accepting.resize(states);
            for (auto& row : automaton.next) for (int& next : row) next = random() % states;
            for (char& accepting : automaton.accepting) accepting = random() & 1;
            std::vector<int> text(9);
            for (int& symbol : text) symbol = random() & 1;

            poe::acceptedsubstringstats expected;
            for (int left = 0; left < static_cast<int>(text.size()); ++left) {
                int state = automaton.start;
                for (int right = left; right < static_cast<int>(text.size()); ++right) {
                    state = automaton.next[state][text[right]];
                    if (automaton.accepting[state]) {
                        ++expected.count;
                        expected.longest = std::max(expected.longest, right - left + 1);
                    }
                }
            }
            const auto actual = poe::accepted_substring_stats(automaton, text);
            assert(actual.longest == expected.longest && actual.count == expected.count);
        }
    }

    for (int truth_table = 0; truth_table < 16; ++truth_table) {
        std::vector operation(2, std::vector<int>(2));
        operation[0][0] = truth_table >> 3 & 1;
        operation[0][1] = truth_table >> 2 & 1;
        operation[1][0] = truth_table >> 1 & 1;
        operation[1][1] = truth_table & 1;
        auto oracle = [&](const std::vector<int>& word) {
            return !word.empty() && (poe::parenthesized_results(word, operation) >> 1 & 1);
        };
        const auto learned = poe::right_context_dfa(2, 3, oracle);
        assert(learned.size() <= 7);
        for (int length = 0; length <= 7; ++length) {
            for (int mask = 0; mask < (1 << length); ++mask) {
                std::vector<int> word(length);
                for (int index = 0; index < length; ++index) word[index] = mask >> index & 1;
                assert(learned.accepts(word) == oracle(word));
            }
        }
        for (int trial = 0; trial < 100; ++trial) {
            const int length = 8 + random() % 7;
            std::vector<int> word(length);
            for (int& symbol : word) symbol = random() & 1;
            assert(learned.accepts(word) == oracle(word));
        }
    }
}
