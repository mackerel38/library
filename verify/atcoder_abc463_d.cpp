#include <bits/stdc++.h>
#include "algorithm/binarysearch.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, required;
    cin >> n >> required;
    vector<pair<long long, long long>> intervals(n);
    for (auto& [left, right] : intervals) cin >> left >> right;
    ranges::sort(intervals, {}, [](const auto& interval) { return interval.second; });

    auto feasible = [&](long long extra) {
        int selected = 0;
        long long next_left = 0;
        for (const auto& [left, right] : intervals) {
            if (next_left <= left) {
                ++selected;
                next_left = right + extra + 1;
            }
        }
        return selected >= required;
    };
    const auto extra = last_true(0LL, 1'000'000'001LL, feasible);
    cout << (extra ? *extra + 1 : -1) << '\n';
}
