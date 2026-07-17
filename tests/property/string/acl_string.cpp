#include "string/string.hpp"

std::vector<int> naive_suffix_array(const std::string& text) {
    std::vector<int> result(text.size());
    std::iota(result.begin(), result.end(), 0);
    std::sort(result.begin(), result.end(), [&](int left, int right) {
        return text.substr(left) < text.substr(right);
    });
    return result;
}

int main() {
    std::mt19937 random(13579);
    for (int n = 0; n <= 150; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            std::string text(n, 'a');
            for (char& character : text) {
                character = static_cast<char>('a' + random() % 5);
            }
            const auto suffix = poe::suffix_array(text);
            assert(suffix == naive_suffix_array(text));
            const auto lcp = poe::lcp_array(text, suffix);
            if (n > 0) {
                for (int i = 0; i + 1 < n; ++i) {
                    int expected = 0;
                    while (suffix[i] + expected < n && suffix[i + 1] + expected < n &&
                           text[suffix[i] + expected] == text[suffix[i + 1] + expected]) {
                        ++expected;
                    }
                    assert(lcp[i] == expected);
                }
            }
            const auto z = poe::z_algorithm(text);
            for (int i = 0; i < n; ++i) {
                int expected = 0;
                while (i + expected < n && text[expected] == text[i + expected]) {
                    ++expected;
                }
                assert(z[i] == expected);
            }

            const auto prefix = poe::prefix_function(text);
            for (int i = 0; i < n; ++i) {
                int expected = 0;
                for (int length = 1; length <= i; ++length) {
                    if (text.substr(0, length) == text.substr(i + 1 - length, length))
                        expected = length;
                }
                assert(prefix[i] == expected);
            }

            const auto odd = poe::manacher_odd(text);
            const auto even = poe::manacher_even(text);
            for (int center = 0; center < n; ++center) {
                int expected_odd = 1;
                while (center - expected_odd >= 0 && center + expected_odd < n &&
                       text[center - expected_odd] == text[center + expected_odd])
                    ++expected_odd;
                assert(odd[center] == expected_odd);
                int expected_even = 0;
                while (center - expected_even - 1 >= 0 && center + expected_even < n &&
                       text[center - expected_even - 1] == text[center + expected_even])
                    ++expected_even;
                assert(even[center] == expected_even);
            }

            std::string pattern(random() % 8, 'a');
            for (char& character : pattern)
                character = static_cast<char>('a' + random() % 5);
            std::vector<int> expected_positions;
            for (int position = 0; position + static_cast<int>(pattern.size()) <= n;
                 ++position) {
                if (text.compare(position, pattern.size(), pattern) == 0)
                    expected_positions.push_back(position);
            }
            assert(poe::find_all(text, pattern) == expected_positions);

            poe::suffixquery query(text);
            assert(query.suffix_array() == suffix);
            assert(query.contains(pattern) ==
                   (text.find(pattern) != std::string::npos));
            if (n > 0) {
                const int first = random() % n;
                const int second = random() % n;
                int expected_lcp = 0;
                while (first + expected_lcp < n && second + expected_lcp < n &&
                       text[first + expected_lcp] == text[second + expected_lcp])
                    ++expected_lcp;
                assert(query.lcp(first, second) == expected_lcp);
            }
            int first_left = random() % (n + 1);
            int first_right = random() % (n + 1);
            int second_left = random() % (n + 1);
            int second_right = random() % (n + 1);
            if (first_left > first_right) std::swap(first_left, first_right);
            if (second_left > second_right) std::swap(second_left, second_right);
            const int expected_compare = text.substr(first_left, first_right - first_left)
                                             .compare(text.substr(second_left,
                                                                  second_right - second_left));
            assert((query.compare(first_left, first_right, second_left, second_right) < 0) ==
                   (expected_compare < 0));
            assert((query.compare(first_left, first_right, second_left, second_right) == 0) ==
                   (expected_compare == 0));
        }
    }
}
