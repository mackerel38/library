#include <bits/stdc++.h>
#include "algorithm/intervalcover.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(0);
    for (int trial = 0; trial < 3000; ++trial) {
        const int length = 1 + random() % 6;
        const int count = 1 + random() % 8;
        vector<pair<int, int>> intervals(count);
        int union_mask = 0;
        for (auto& [left, right] : intervals) {
            left = random() % length;
            right = left + 1 + random() % (length - left);
            for (int x = left; x < right; ++x) union_mask |= 1 << x;
        }
        vector<modint998244353> expected(count + 1);
        vector<char> covers(1 << count);
        for (int subset = 0; subset < (1 << count); ++subset) {
            int covered = 0;
            for (int i = 0; i < count; ++i) {
                if (subset >> i & 1) {
                    for (int x = intervals[i].first; x < intervals[i].second; ++x) {
                        covered |= 1 << x;
                    }
                }
            }
            covers[subset] = covered == (1 << length) - 1;
            if (covers[subset]) ++expected[popcount(static_cast<unsigned>(subset))];
        }
        assert(interval_cover_subset_counts(length, intervals) == expected);
        if (union_mask != (1 << length) - 1) continue;

        vector<modint998244353> expectation(1 << count);
        for (int used = count; used >= 0; --used) {
            for (int subset = 0; subset < (1 << count); ++subset) {
                if (popcount(static_cast<unsigned>(subset)) != used || covers[subset]) continue;
                modint998244353 value = count;
                for (int item = 0; item < count; ++item) {
                    if (!(subset >> item & 1)) value += expectation[subset | (1 << item)];
                }
                expectation[subset] = value / (count - used);
            }
        }
        assert(expected_random_interval_cover_time(length, intervals) == expectation[0]);
    }
}
