#include <bits/stdc++.h>
#include "algorithm/rangesort.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(237);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = 1 + random() % 20;
        vector<int> initial(n);
        iota(initial.begin(), initial.end(), 0);
        ranges::shuffle(initial, random);
        auto values = initial;
        const int target = random() % n;
        vector<rangesortquery> queries;
        const int q = random() % 30;
        for (int query = 0; query < q; ++query) {
            const int left = random() % n;
            const int right = left + 1 + random() % (n - left);
            const bool ascending = random() & 1;
            queries.push_back({left, right, ascending});
            if (ascending) ranges::sort(values.begin() + left, values.begin() + right);
            else ranges::sort(values.begin() + left, values.begin() + right, greater{});
        }
        const int expected = ranges::find(values, target) - values.begin();
        assert(position_after_range_sorts(initial, target, queries) == expected);
    }
}
