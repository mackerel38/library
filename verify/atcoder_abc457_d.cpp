#include <bits/stdc++.h>
#include "algorithm/binarysearch.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long operations;
    cin >> n >> operations;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    auto possible = [&](long long minimum) {
        long long used = 0;
        for (int i = 0; i < n; ++i) {
            if (values[i] >= minimum) continue;
            used += (minimum - values[i] + i) / (i + 1);
            if (used > operations) return false;
        }
        return true;
    };
    cout << *last_true<long long>(1, values[0] + operations + 1, possible) << '\n';
}
