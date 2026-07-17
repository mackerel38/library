#include <algorithm>
#include <cassert>
#include <random>
#include <set>
#include <string>

#include "string/distinctsubstring.hpp"
#include "string/subsequence.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = random() % 9;
        std::string text(n, 'a');
        for (char& character : text) character = "ab#"[random() % 3];
        std::set<std::string> distinct;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right <= n; ++right) {
                const std::string part = text.substr(left, right - left);
                if (!part.contains('#')) distinct.insert(part);
            }
        }
        assert(poe::count_distinct_substrings_without(text, '#') ==
               static_cast<long long>(distinct.size()));

        const int m = 1 + random() % 4;
        std::string pattern(m, 'a');
        for (char& character : pattern) character = "ab"[random() % 2];
        long long brute = 0;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right <= n; ++right) {
                int matched = 0;
                for (int i = left; i < right && matched < m; ++i)
                    matched += text[i] == pattern[matched];
                brute += matched < m;
            }
        }
        assert(poe::count_substrings_without_subsequence(text, pattern) == brute);
    }
}
