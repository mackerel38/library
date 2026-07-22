#define PROBLEM "https://atcoder.jp/contests/abc272/tasks/abc272_h"
#include <bits/stdc++.h>
#include "dp/permutationthreshold.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> thresholds(n);
    for (int index = 0; index < n; ++index) {
        int length;
        cin >> length;
        thresholds[index] = n - 1 - length;
    }
    const auto counts = permutation_threshold_counts<998244353>(thresholds);
    modint998244353 even = 0;
    for (int flips = 0; flips <= n; flips += 2) even += counts[flips];
    modint998244353 factorial = 1;
    for (int value = 1; value <= n; ++value) factorial *= value;
    cout << (even * n / factorial).val() << '\n';
}
