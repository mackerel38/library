#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>

#include "dp/optimization.hpp"
#include "dp/sequence.hpp"
#include "dp/subsetsum.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = random() % 12;
        std::vector<int> values(n);
        for (int& value : values) value = random() % 11;
        const auto possible = poe::subsetsum(values, 50);
        std::vector<bool> brute(51);
        brute[0] = true;
        for (int value : values) {
            for (int sum = 50; sum >= value; --sum) brute[sum] = brute[sum] || brute[sum - value];
        }
        assert(possible == brute);

        std::vector<int> length(n, 1);
        int answer = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (values[j] < values[i]) length[i] = std::max(length[i], length[j] + 1);
            }
            answer = std::max(answer, length[i]);
        }
        assert(poe::lis(values).size() == answer);
    }

    for (int trial = 0; trial < 500; ++trial) {
        const int n = random() % 10;
        std::vector<std::pair<int, int>> intervals(n);
        for (auto& [left, right] : intervals) {
            left = random() % 10;
            right = left + 1 + random() % 10;
        }
        std::vector<int> length(n, 1);
        int answer = 0;
        std::vector<int> order(n);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int first, int second) {
            const auto [first_left, first_right] = intervals[first];
            const auto [second_left, second_right] = intervals[second];
            return first_right != second_right ? first_right > second_right
                                               : first_left > second_left;
        });
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                const auto [outer_left, outer_right] = intervals[order[j]];
                const auto [inner_left, inner_right] = intervals[order[i]];
                if (outer_left < inner_left && inner_right < outer_right) {
                    length[i] = std::max(length[i], length[j] + 1);
                }
            }
            answer = std::max(answer, length[i]);
        }
        const auto result = poe::longest_nested_intervals(intervals);
        assert(result.size() == answer);
        for (int i = 1; i < result.size(); ++i) {
            const auto [outer_left, outer_right] = intervals[result.indices[i - 1]];
            const auto [inner_left, inner_right] = intervals[result.indices[i]];
            assert(outer_left < inner_left && inner_right < outer_right);
        }
    }

    for (int trial = 0; trial < 200; ++trial) {
        const int n = 1 + random() % 12;
        std::vector<long long> weights(n);
        for (auto& weight : weights) weight = random() % 20;
        std::vector prefix(n + 1, 0LL);
        for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + weights[i];
        std::vector brute(n + 1, std::vector<long long>(n + 1));
        for (int length = 2; length <= n; ++length) {
            for (int left = 0; left + length <= n; ++left) {
                const int right = left + length;
                brute[left][right] = (1LL << 60);
                for (int middle = left + 1; middle < right; ++middle) {
                    brute[left][right] = std::min(
                        brute[left][right],
                        brute[left][middle] + brute[middle][right] + prefix[right] - prefix[left]
                    );
                }
            }
        }
        assert(poe::optimal_merge_cost(weights) == brute[0][n]);
    }

    const std::string alphabet = "ABCX";
    const std::string pattern = "ABC";
    for (int trial = 0; trial < 120; ++trial) {
        const int n = 3 + random() % 5;
        std::string text(n, 'A');
        for (char& character : text) character = alphabet[random() % alphabet.size()];
        auto count_occurrences = [&](const std::string& value) {
            int count = 0;
            for (int i = 0; i + 3 <= n; ++i) count += value.substr(i, 3) == pattern;
            return count;
        };
        const int original = count_occurrences(text);
        for (int increase = 0; increase <= 2; ++increase) {
            int brute = 100;
            int ways = 1;
            for (int i = 0; i < n; ++i) ways *= static_cast<int>(alphabet.size());
            for (int mask = 0; mask < ways; ++mask) {
                int code = mask;
                std::string result(n, 'A');
                int changes = 0;
                for (int i = 0; i < n; ++i) {
                    result[i] = alphabet[code % alphabet.size()];
                    code /= static_cast<int>(alphabet.size());
                    changes += result[i] != text[i];
                }
                if (count_occurrences(result) == original + increase) {
                    brute = std::min(brute, changes);
                }
            }
            if (brute == 100) brute = -1;
            assert(poe::minimum_unbordered_pattern_increase(text, pattern, increase) == brute);
        }
    }
}
