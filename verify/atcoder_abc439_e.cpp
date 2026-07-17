#include <bits/stdc++.h>
#include "dp/sequence.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<long long, long long>> segments(n);
    for (auto& [lower, upper] : segments) cin >> lower >> upper;
    ranges::sort(segments, [](const auto& left, const auto& right) {
        return left.first != right.first ? left.first < right.first : left.second > right.second;
    });
    vector<long long> upper(n);
    for (int i = 0; i < n; ++i) upper[i] = segments[i].second;
    cout << lis(upper).indices.size() << '\n';
}
