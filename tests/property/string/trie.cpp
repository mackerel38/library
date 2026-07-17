#include "string/trie.hpp"

int main() {
    std::mt19937 random(731849);
    for (int trial = 0; trial < 1000; ++trial) {
        const int pattern_count = 1 + random() % 10;
        std::vector<std::string> patterns(pattern_count);
        poe::trie<char> dictionary;
        poe::ahocorasick<char> automaton('a', 'd');
        for (std::string& pattern : patterns) {
            const int length = 1 + random() % 8;
            pattern.resize(length);
            for (char& symbol : pattern) symbol = static_cast<char>('a' + random() % 4);
            dictionary.add(pattern);
            automaton.add(pattern);
        }
        automaton.build();
        const int text_length = random() % 80;
        std::string text(text_length, 'a');
        for (char& symbol : text) symbol = static_cast<char>('a' + random() % 4);
        const auto actual_positions = automaton.find_all(text);
        const auto actual_count = automaton.count(text);
        for (int id = 0; id < pattern_count; ++id) {
            std::vector<int> expected;
            for (int position = 0;
                 position + static_cast<int>(patterns[id].size()) <= text_length;
                 ++position) {
                if (text.compare(position, patterns[id].size(), patterns[id]) == 0) {
                    expected.push_back(position);
                }
            }
            assert(actual_positions[id] == expected);
            assert(actual_count[id] == static_cast<long long>(expected.size()));
            assert(dictionary.count(patterns[id]) ==
                   static_cast<int>(std::ranges::count(patterns, patterns[id])));
        }
        for (const std::string& pattern : patterns) {
            for (int length = 0; length <= static_cast<int>(pattern.size()); ++length) {
                const std::string prefix = pattern.substr(0, length);
                const int expected = std::ranges::count_if(patterns, [&](const auto& word) {
                    return word.starts_with(prefix);
                });
                assert(dictionary.prefix_count(prefix) == expected);
            }
        }
    }
}
