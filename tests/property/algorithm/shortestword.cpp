#include <cassert>
#include <optional>
#include <random>
#include <string>
#include <vector>
#include "algorithm/shortestword.hpp"

using namespace poe;

int main() {
    std::mt19937 random(443);
    for (int state_count = 1; state_count <= 7; ++state_count) {
        for (int trial = 0; trial < 300; ++trial) {
            std::vector<std::array<int, 2>> transition(state_count);
            for (auto& next : transition) {
                next = {static_cast<int>(random() % state_count),
                        static_cast<int>(random() % state_count)};
            }
            const int goal = random() % state_count;
            const auto actual = shortest_lexicographic_word<char>(
                state_count,
                0,
                [&](int state) { return state == goal; },
                [&](int state, auto relax) {
                    relax(transition[state][0], 'a');
                    relax(transition[state][1], 'b');
                }
            );
            std::optional<std::string> expected;
            for (int length = 0; length < state_count && !expected; ++length) {
                for (int mask = 0; mask < (1 << length); ++mask) {
                    int state = 0;
                    std::string word;
                    for (int position = 0; position < length; ++position) {
                        const int symbol = (mask >> (length - 1 - position)) & 1;
                        word.push_back(static_cast<char>('a' + symbol));
                        state = transition[state][symbol];
                    }
                    if (state == goal) {
                        expected = word;
                        break;
                    }
                }
            }
            if (!expected) {
                assert(!actual);
            } else {
                assert(actual && std::string(actual->begin(), actual->end()) == *expected);
            }
        }
    }
}
