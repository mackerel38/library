#include "string/palindromictree.hpp"

bool palindrome(const std::string& text) {
    return std::equal(text.begin(), text.begin() + text.size() / 2, text.rbegin());
}

int main() {
    std::mt19937 random(912347);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = random() % 14;
        std::string text(n, 'a');
        for (char& symbol : text) symbol = static_cast<char>('a' + random() % 4);
        poe::palindromictree<char> tree(text);
        std::map<std::string, int> expected;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right <= n; ++right) {
                const std::string part = text.substr(left, right - left);
                if (palindrome(part)) ++expected[part];
            }
        }
        assert(tree.distinct_palindromes() == static_cast<int>(expected.size()));
        int longest = 0;
        for (int state = 2; state < tree.size(); ++state) {
            const auto symbols = tree.palindrome(state);
            const std::string value(symbols.begin(), symbols.end());
            assert(expected.contains(value));
            assert(tree[state].occurrence == expected[value]);
            longest = std::max(longest, static_cast<int>(value.size()));
        }
        assert(tree.longest_palindrome().length() == longest);
        for (int position = 0; position < n; ++position) {
            const int state = tree.suffix_state(position);
            const auto symbols = tree.palindrome(state);
            const std::string value(symbols.begin(), symbols.end());
            assert(text.substr(position + 1 - value.size(), value.size()) == value);
        }

        std::vector<int> dp(n + 1, n + 1);
        dp[0] = 0;
        for (int right = 1; right <= n; ++right) {
            for (int left = 0; left < right; ++left) {
                if (palindrome(text.substr(left, right - left))) {
                    dp[right] = std::min(dp[right], dp[left] + 1);
                }
            }
        }
        const auto partition = tree.minimum_partition();
        assert(partition.count == dp[n]);
        int covered = 0;
        for (auto [left, right] : partition.ranges) {
            assert(left == covered && palindrome(text.substr(left, right - left)));
            covered = right;
        }
        assert(covered == n);
    }
}
