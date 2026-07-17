#include "string/suffixautomaton.hpp"

int main() {
    std::mt19937 random(518273);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = random() % 12;
        std::string text(n, 'a');
        for (char& symbol : text) symbol = static_cast<char>('a' + random() % 4);
        poe::suffixautomaton<char> automaton(text);
        std::map<std::string, int> substrings;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right <= n; ++right) {
                ++substrings[text.substr(left, right - left)];
            }
        }
        assert(automaton.distinct_substrings() == substrings.size());
        int repeated = 0;
        int index = 0;
        for (const auto& [substring, count] : substrings) {
            assert(automaton.contains(substring));
            assert(automaton.count(substring) == count);
            const auto kth = automaton.kth(index++);
            assert(kth && std::string(kth->begin(), kth->end()) == substring);
            if (count >= 2) repeated = std::max(repeated, static_cast<int>(substring.size()));
        }
        assert(!automaton.kth(substrings.size()));
        assert(automaton.longest_repeated_substring() == repeated);

        const int m = random() % 12;
        std::string other(m, 'a');
        for (char& symbol : other) symbol = static_cast<char>('a' + random() % 4);
        int expected = 0;
        for (int left = 0; left < m; ++left) {
            for (int right = left + 1; right <= m; ++right) {
                if (text.find(other.substr(left, right - left)) != std::string::npos) {
                    expected = std::max(expected, right - left);
                }
            }
        }
        const auto common = automaton.longest_common_substring(other);
        assert(common.length == expected);
        assert(0 <= common.begin() && common.begin() <= common.end &&
               common.end <= m);
        assert(text.find(other.substr(common.begin(), common.length)) != std::string::npos);
    }
}
