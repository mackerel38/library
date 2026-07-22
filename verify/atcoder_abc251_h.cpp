#include <bits/stdc++.h>
#include "math/binomialsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    int m, k;
    cin >> n >> m >> k;
    vector<pair<int, long long>> runs(m);
    for (auto& [value, count] : runs) cin >> value >> count;
    const auto answer = adjacent_sum_reduce_rle<7>(runs, n - k);
    bool first = true;
    for (const auto [value, count] : answer) {
        for (long long repeat = 0; repeat < count; ++repeat) {
            if (!first) cout << ' ';
            first = false;
            cout << value;
        }
    }
    cout << '\n';
}
