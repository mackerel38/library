#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>
#include "string/persistentsequence.hpp"
int main() {
    std::mt19937 random(437);
    for (int size = 1; size <= 200; ++size) {
        poe::persistentsequenceorder<int> sequences;
        std::vector<std::vector<int>> words(1);
        for (int i = 1; i <= size; ++i) {
            const int parent = random() % i;
            words.push_back(words[parent]);
            words.back().push_back(random() % 5);
            assert(sequences.add(parent, words.back().back()) == i);
        }
        std::vector<int> expected(size);
        std::iota(expected.begin(), expected.end(), 1);
        std::ranges::sort(expected, [&](int a, int b) { return words[a] != words[b] ? words[a] < words[b] : a < b; });
        assert(sequences.order() == expected);
    }
    for (int trial = 0; trial < 100; ++trial) {
        constexpr int limit = 100;
        poe::persistentconcatenation<int> sequences({0, 1, 2}, limit);
        std::vector<std::vector<int>> values{{0}, {1}, {2}};
        for (int iteration = 0; iteration < 300; ++iteration) {
            const int left = random() % values.size();
            const int right = random() % values.size();
            auto joined = values[left];
            joined.insert(joined.end(), values[right].begin(), values[right].end());
            if (joined.size() > limit) joined.resize(limit);
            const int version = sequences.add(left, right);
            values.push_back(joined);
            assert(version == static_cast<int>(values.size()) - 1);
            assert(sequences.length(version) == joined.size());
            for (int index = 0; index < static_cast<int>(joined.size()); ++index) {
                assert(sequences.get(version, index) == joined[index]);
            }
        }
    }
}
