#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>

#include "algorithm/permutation.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260716);
    for (int n = 1; n <= 9; ++n) {
        for (int repetition = 0; repetition < 150; ++repetition) {
            vector<long long> values(n);
            for (auto& value : values) value = random() % 6;
            const long long maximum_drop = random() % 7;
            vector<long long> permutation = values;
            ranges::sort(permutation);
            int expected = 0;
            do {
                bool valid = true;
                for (int index = 0; index + 1 < n; ++index) {
                    valid &= permutation[index + 1] >= permutation[index] - maximum_drop;
                }
                expected += valid;
            } while (ranges::next_permutation(permutation).found);
            assert(count_permutations_max_drop<998244353>(values, maximum_drop).val()
                   == static_cast<unsigned>(expected));
        }
    }

    for (int n = 2; n <= 8; ++n) {
        const int count = n * (n - 1) / 2;
        for (int repetition = 0; repetition < 300; ++repetition) {
            vector<int> values(n);
            for (int& value : values) value = random() % 5;
            vector<vector<int>> sorted;
            for (int left = 0; left < n; ++left) {
                for (int right = left + 1; right < n; ++right) {
                    auto swapped = values;
                    ranges::swap(swapped[left], swapped[right]);
                    sorted.push_back(move(swapped));
                }
            }
            ranges::sort(sorted);
            vector<long long> ranks(count);
            iota(ranks.begin(), ranks.end(), 0);
            ranges::shuffle(ranks, random);
            const auto result = one_swap_lexicographic_pairs(values, ranks);
            for (int query = 0; query < count; ++query) {
                auto swapped = values;
                const auto [left, right] = result[query];
                ranges::swap(swapped[left], swapped[right]);
                assert(swapped == sorted[ranks[query]]);
            }
        }
    }

    for (int n = 1; n <= 8; ++n) {
        for (int mask = 0; mask < (1 << (n - 1)); ++mask) {
            vector<int> forward(n - 1);
            for (int index = 0; index + 1 < n; ++index) forward[index] = mask >> index & 1;
            vector<set<int>> possible(n);
            vector<int> permutation(n);
            iota(permutation.begin(), permutation.end(), 0);
            do {
                vector<int> position(n);
                for (int index = 0; index < n; ++index) position[permutation[index]] = index;
                bool valid = true;
                for (int value = 0; value + 1 < n; ++value) {
                    valid &= (position[value] < position[value + 1]) == static_cast<bool>(forward[value]);
                }
                if (valid) {
                    for (int value = 0; value < n; ++value) possible[value].insert(position[value]);
                }
            } while (ranges::next_permutation(permutation).found);
            const auto actual = path_order_positions(forward, identity{});
            vector<int> expected_counts(n);
            for (int value = 0; value < n; ++value) {
                assert(actual.ranges[value] == pair(*possible[value].begin(), *possible[value].rbegin()));
                assert(possible[value].size() ==
                       static_cast<size_t>(actual.ranges[value].second - actual.ranges[value].first + 1));
                for (int position : possible[value]) ++expected_counts[position];
            }
            assert(actual.counts == expected_counts);
        }
    }
}
