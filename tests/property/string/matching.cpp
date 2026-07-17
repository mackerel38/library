#include "string/matching.hpp"

int main() {
    std::mt19937 random(20260716);
    const std::string alphabet = "abc_";
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = random() % 35;
        const int m = random() % 40;
        std::string text(n, 'a'), pattern(m, 'a');
        for (char& symbol : text) symbol = alphabet[random() % alphabet.size()];
        for (char& symbol : pattern) symbol = alphabet[random() % alphabet.size()];
        std::vector<int> expected;
        for (int start = 0; start + m <= n; ++start) {
            bool match = true;
            for (int index = 0; index < m; ++index) {
                if (
                    text[start + index] != '_' && pattern[index] != '_'
                    && text[start + index] != pattern[index]
                ) {
                    match = false;
                }
            }
            if (match) expected.push_back(start);
        }
        if (m == 0) {
            expected.resize(static_cast<std::size_t>(n) + 1);
            std::iota(expected.begin(), expected.end(), 0);
        }
        assert(poe::wildcard_matches(text, pattern) == expected);
    }
}

