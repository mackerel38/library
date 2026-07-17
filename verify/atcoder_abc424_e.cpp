#define PROBLEM "https://atcoder.jp/contests/abc424/tasks/abc424_e"
#include <bits/stdc++.h>
#include "algorithm/balancedsplit.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << fixed << setprecision(20);
    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        long long operations, rank;
        cin >> n >> operations >> rank;
        vector<long long> lengths(n);
        for (auto& length : lengths) cin >> length;
        cout << kth_largest_after_max_halving(lengths, operations, rank) << '\n';
    }
}
