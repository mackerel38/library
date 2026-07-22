#include <cassert>
#include <string>
#include <vector>

#include "math/modint.hpp"
#include "string/avoid.hpp"

int main() {
    using mint = poe::modint998244353;
    const std::vector<std::vector<std::string>> pattern_sets{
        {"a"}, {"aa"}, {"ab", "ba"}, {"aba", "bb"}, {"aaa", "bab", "bba"}
    };
    for (const auto& patterns : pattern_sets) {
        for (int length = 0; length <= 10; ++length) {
            int brute = 0;
            for (int mask = 0; mask < (1 << length); ++mask) {
                std::string word(length, 'a');
                for (int i = 0; i < length; ++i) if (mask >> i & 1) word[i] = 'b';
                bool valid = true;
                for (const auto& pattern : patterns) {
                    if (word.find(pattern) != std::string::npos) valid = false;
                }
                brute += valid;
            }
            assert(poe::count_avoiding_words<mint>(length, patterns, 'a', 'b').val() ==
                   static_cast<unsigned>(brute));
        }
    }

    std::mt19937 random(268);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int length = static_cast<int>(random() % 13);
        std::string text(length, 'a');
        for (char& symbol : text) symbol += static_cast<char>(random() % 3);
        const int count = static_cast<int>(random() % 6);
        std::vector<std::string> patterns(count);
        for (auto& pattern : patterns) {
            pattern.resize(1 + random() % 5, 'a');
            for (char& symbol : pattern) symbol += static_cast<char>(random() % 3);
        }
        const auto actual = poe::minimum_pattern_replacements(text, patterns);
        std::vector<std::pair<int, int>> occurrences;
        for (const auto& pattern : patterns) {
            for (int begin = 0; begin + static_cast<int>(pattern.size()) <= length; ++begin) {
                if (text.compare(begin, pattern.size(), pattern) == 0) {
                    occurrences.emplace_back(begin, begin + static_cast<int>(pattern.size()));
                }
            }
        }
        int expected = length + 1;
        for (int mask = 0; mask < (1 << length); ++mask) {
            bool valid = true;
            for (const auto [begin, end] : occurrences) {
                bool hit = false;
                for (int position = begin; position < end; ++position) hit |= mask >> position & 1;
                valid &= hit;
            }
            if (valid) expected = std::min(expected, std::popcount(static_cast<unsigned int>(mask)));
        }
        assert(static_cast<int>(actual.size()) == expected);
        for (const auto [begin, end] : occurrences) {
            assert(std::ranges::any_of(actual, [&](int position) {
                return begin <= position && position < end;
            }));
        }
    }
}
