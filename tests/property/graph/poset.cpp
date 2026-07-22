#include <bits/stdc++.h>
#include "graph/poset.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(2370);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = random() % 10;
        vector relation(n, vector<char>(n));
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right < n; ++right) {
                relation[left][right] = random() % 3 == 0;
            }
        }
        for (int middle = 0; middle < n; ++middle) {
            for (int left = 0; left < n; ++left) {
                for (int right = 0; right < n; ++right) {
                    relation[left][right] |= relation[left][middle] && relation[middle][right];
                }
            }
        }
        int expected = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool valid = true;
            for (int left = 0; left < n; ++left) {
                for (int right = left + 1; right < n; ++right) {
                    if ((mask >> left & 1) && (mask >> right & 1)
                        && (relation[left][right] || relation[right][left])) valid = false;
                }
            }
            if (valid) expected = max(expected, popcount(static_cast<unsigned>(mask)));
        }
        const auto result = maximum_poset_antichain(n, [&](int left, int right) {
            return relation[left][right];
        });
        assert(result.size() == expected);
        assert(static_cast<int>(result.chains.size()) == expected);
        vector<int> seen(n);
        for (const auto& chain : result.chains) {
            for (int index = 0; index < static_cast<int>(chain.size()); ++index) {
                assert(!seen[chain[index]]++);
                if (index > 0) assert(relation[chain[index - 1]][chain[index]]);
            }
        }
        assert(ranges::all_of(seen, [](int count) { return count == 1; }));
    }
}
